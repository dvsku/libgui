#include "libgui/window/window_context.hpp"
#include "libgui/global.hpp"
#include "libgui/exceptions/libgui_exception.hpp"

#include <stdexcept>
#include <versionhelpers.h>

using namespace libgui;
using namespace libgui::internals;

// Message sent when user clicks icon in system tray
#define WM_USER_TRAYICON WM_USER + 1

///////////////////////////////////////////////////////////////////////////////
// INTERNAL
///////////////////////////////////////////////////////////////////////////////

static std::string internal_get_glfw_failure();

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

window_context::window_context(window* window, const window_settings& settings)
    : m_wnd(window), m_settings(settings)
{
    global::backend.initialize();

    m_glfw_handle = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), NULL, NULL);
    if (!m_glfw_handle) {
        LIBGUI_EXCEPTION_THROW("Failed to create window. | {}", internal_get_glfw_failure());
    }

    auto win32_handle = get_win32_handle();
    if (!win32_handle) {
        LIBGUI_EXCEPTION_THROW("Failed to create window.");
    }

    glfwMakeContextCurrent(m_glfw_handle);
    glfwSwapInterval(1);

    SetWindowLongPtr(win32_handle, GWLP_USERDATA, (intptr_t)this);
    glfwSetWindowUserPointer(m_glfw_handle, this);
}

window_context::~window_context() {
    teardown();
}

bool window_context::initialize() {
    if (!m_glfw_handle)
        return false;

    auto win32_handle = get_win32_handle();
    if (!win32_handle)
        return false;

    // Save default callback
    m_wndproc_default_callback = (WNDPROC)GetWindowLongPtr(win32_handle, GWLP_WNDPROC);
    if (!m_wndproc_default_callback)
        return false;

    // Save standard style
    m_windowed_wnd_style = GetWindowLongPtr(win32_handle, GWL_STYLE);

    // Save borderless style
    m_borderless_wnd_style = m_windowed_wnd_style;
    m_borderless_wnd_style |= WS_OVERLAPPEDWINDOW;
    m_borderless_wnd_style |= WS_CAPTION;
    m_borderless_wnd_style |= WS_MAXIMIZEBOX;
    m_borderless_wnd_style |= WS_THICKFRAME;
    m_borderless_wnd_style |= CS_VREDRAW;
    m_borderless_wnd_style |= CS_HREDRAW;

    // Set to windowed
    SetWindowLongPtr(win32_handle, GWL_STYLE,    m_windowed_wnd_style);
    SetWindowLongPtr(win32_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(internals::window_context::internal_wndproc_callback_windowed));

    // Set GLFW callbacks

    glfwSetWindowSizeCallback(m_glfw_handle, window_context::internal_glfw_window_resize_callback);
    glfwSetFramebufferSizeCallback(m_glfw_handle, window_context::internal_glfw_frame_buffer_resize_callback);
    glfwSetScrollCallback(m_glfw_handle, window_context::internal_glfw_scroll_callback);
    glfwSetMouseButtonCallback(m_glfw_handle, window_context::internal_glfw_mouse_btn_callback);
    glfwSetCursorPosCallback(m_glfw_handle, window_context::internal_glfw_mouse_move_callback);
    glfwSetDropCallback(m_glfw_handle, window_context::internal_glfw_drop_callback);

    // Set event callbacks
    internal_initialize_event();

    // Initialize taskbar
    internal_initialize_tb();

    // Initialize system tray
    internal_initialize_st();

    // Apply startup settings

    if (m_settings.center_on_startup) {
        glfwSetWindowMonitor(m_glfw_handle, NULL,
            (GetSystemMetrics(SM_CXSCREEN) / 2) - (m_settings.width / 2),
            (GetSystemMetrics(SM_CYSCREEN) / 2) - (m_settings.height / 2),
            m_settings.width, m_settings.height, GLFW_DONT_CARE);
    }

    if (m_settings.maximize_on_startup) {
        set_maximized(true);
    }

    if (m_settings.minimized_to_st_on_startup) {
        set_minimize_to_st(true);
    }

    // Apply standard settings
    set_settings(m_settings);

    return true;
}

void window_context::teardown() {
    internal_teardown_event();
    internal_teardown_tb();
    internal_teardown_st();
}

GLFWwindow* window_context::get_glfw_handle() {
    return m_glfw_handle;
}

HWND window_context::get_win32_handle() {
    if (!m_glfw_handle)
        return nullptr;

    return glfwGetWin32Window(m_glfw_handle);
}

window_settings* window_context::get_settings() {
    return &m_settings;
}

void window_context::set_settings(const window_settings& settings) {

}

void window_context::event_poll() {
    if (!m_wnd) return;

    glfwPollEvents();
    m_wnd->m_event_system.process();
}

void window_context::event_wait() {
    glfwWaitEvents();
}

void window_context::request_close() {
    if (!m_glfw_handle)
        return;

    glfwSetWindowShouldClose(m_glfw_handle, GLFW_TRUE);
}

void window_context::set_minimize_to_tb(bool value) {
    if (!m_glfw_handle)
        return;

    if (is_minimized_to_st())
        return;

    if (!is_minimized_to_tb() && value) {
        glfwIconifyWindow(m_glfw_handle);
    }
    else if (is_minimized_to_tb() && !value) {
        glfwRestoreWindow(m_glfw_handle);
    }
}

void window_context::set_minimize_to_st(bool value) {
    if (!m_glfw_handle)
        return;

    if (is_minimized_to_tb())
        return;

    auto win32_handle = get_win32_handle();
    if (!win32_handle)
        return;

    if (!is_minimized_to_st() && value) {
        if (!internal_set_st_icon_visible(true)) {
            return;
        }

        if (!ShowWindow(win32_handle, SW_HIDE)) {
            internal_set_st_icon_visible(false);
            return;
        }
    }
    else if (is_minimized_to_st() && !value) {
        if (!internal_set_st_icon_visible(false)) {
            return;
        }

        if (ShowWindow(win32_handle, SW_SHOW)) {
            internal_set_st_icon_visible(true);
            return;
        }

        SetForegroundWindow(win32_handle);
    }
}

void window_context::set_maximized(bool value) {
    if (!m_glfw_handle)
        return;

    if (!is_maximized() && value) {
        glfwMaximizeWindow(m_glfw_handle);
    }
    else if (is_maximized() && !value) {
        glfwRestoreWindow(m_glfw_handle);
    }
}

bool window_context::set_borderless(bool value) {
    if (!m_glfw_handle)
        return false;

    auto win32_handle = get_win32_handle();
    if (!win32_handle)
        return false;

    if (!m_borderless && value) {
        SetWindowLongPtr(win32_handle, GWL_STYLE,    m_borderless_wnd_style);
        SetWindowLongPtr(win32_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(internals::window_context::internal_wndproc_callback_borderless));

        // Force window redraw
        SetWindowPos(win32_handle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
    }
    else if (m_borderless && !value) {
        SetWindowLongPtr(win32_handle, GWL_STYLE,    m_windowed_wnd_style);
        SetWindowLongPtr(win32_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(internals::window_context::internal_wndproc_callback_windowed));

        // Force window redraw
        SetWindowPos(win32_handle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
    }
    else {
        return false;
    }

    m_borderless = value;
    return true;
}

bool window_context::set_resizable(bool value) {
    if (!m_glfw_handle)
        return false;

    if (!m_resizable && value) {
        glfwSetWindowSizeLimits(m_glfw_handle, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE);
    }
    else if (m_resizable && !value) {
        int width  = 0U;
        int height = 0U;

        glfwGetWindowSize(m_glfw_handle, &width, &height);
        glfwSetWindowSizeLimits(m_glfw_handle, width, height, width, height);
    }
    else {
        return false;
    }

    m_resizable = value;
    return true;
}

void window_context::set_tb_state(taskbar_state state) {
    if (!m_glfw_handle || !m_tb_handle)
        return;

    auto win32_handle = get_win32_handle();
    if (!win32_handle)
        return;

    m_tb_handle->SetProgressState(win32_handle, (TBPFLAG)state);
}

void window_context::set_tb_value(uint64_t value) {
    if (!m_glfw_handle || !m_tb_handle)
        return;

    auto win32_handle = get_win32_handle();
    if (!win32_handle)
        return;

    m_tb_handle->SetProgressValue(win32_handle, (ULONGLONG)max(value, 100), 100);
}

bool window_context::is_closing() const {
    if (!m_glfw_handle) return true;
    return glfwWindowShouldClose(m_glfw_handle);
}

bool window_context::is_minimized_to_tb() const {
    return m_minimized_to_tb;
}

bool window_context::is_minimized_to_st() const {
    return m_minimized_to_st;
}

bool window_context::is_maximized() const {
    return m_maximized;
}

bool window_context::is_borderless() const {
    return m_borderless;
}

bool window_context::is_resizable() const {
    return m_resizable;
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE
///////////////////////////////////////////////////////////////////////////////

void window_context::internal_initialize_st() {
    if (m_st_handle || !m_glfw_handle || !m_wnd)
        return;

    auto win32_handle = get_win32_handle();
    if (!win32_handle)
        return;

    m_st_handle = new NOTIFYICONDATA();
    m_st_handle->cbSize           = sizeof(NOTIFYICONDATA);
    m_st_handle->hWnd             = win32_handle;
    m_st_handle->uID              = 100;
    m_st_handle->uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    m_st_handle->uCallbackMessage = WM_USER_TRAYICON;
    m_st_handle->hIcon            = (HICON)GetClassLongPtr(win32_handle, GCLP_HICON);

    strncpy(m_st_handle->szTip, m_settings.title.c_str(),
        min(sizeof(m_st_handle->szTip), m_settings.title.size()));
}

void window_context::internal_teardown_st() {
    if (!m_st_handle)
        return;

    internal_set_st_icon_visible(false);

    delete m_st_handle;
    m_st_handle = nullptr;
}

void window_context::internal_initialize_tb() {
    if (!IsWindows7OrGreater() || m_tb_handle)
        return;

    HRESULT result = CoInitialize(NULL);
    if ((result != S_OK) && (result != S_FALSE))
        return;

    result = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_tb_handle));
    if (!SUCCEEDED(result)) {
        return internal_teardown_tb();
    }

    result = m_tb_handle->HrInit();
    if (!SUCCEEDED(result)) {
        return internal_teardown_tb();
    }
}

void window_context::internal_teardown_tb() {
    if (!m_tb_handle)
        return;

    m_tb_handle->Release();
    m_tb_handle = nullptr;
}

void window_context::internal_initialize_event() {
    internal_event_attach<libgui::ev::ev_close>([this](const auto& event) {
        internal_event_callback(event);
    });

    internal_event_attach<libgui::ev::ev_minimize>([this](const auto& event) {
        internal_event_callback(event);
    });

    internal_event_attach<libgui::ev::ev_minimize_restore>([this](const auto& event) {
        internal_event_callback(event);
    });

    internal_event_attach<libgui::ev::ev_maximize>([this](const auto& event) {
        internal_event_callback(event);
    });

    internal_event_attach<libgui::ev::ev_maximize_restore>([this](const auto& event) {
        internal_event_callback(event);
    });

    internal_event_attach<libgui::internals::ev::ev_restore>([this](const auto& event) {
        internal_event_callback(event);
    });

    internal_event_attach<libgui::internals::ev::ev_frame_buffer_resized>([this](const auto& event) {
        internal_event_callback(event);
    });

    internal_event_attach<libgui::internals::ev::ev_update_settings>([this](const auto& event) {
        internal_event_callback(event);
    });
}

void window_context::internal_teardown_event() {

}

bool window_context::internal_set_st_icon_visible(bool value) {
    if (!m_st_handle)
        return false;

    if (!m_st_icon_visible && value) {
        Shell_NotifyIcon(NIM_ADD, m_st_handle);
    }
    else if (m_st_icon_visible && !value) {
        Shell_NotifyIcon(NIM_DELETE, m_st_handle);
    }
    else {
        return false;
    }

    m_st_icon_visible = value;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// WNDPROC

LRESULT window_context::internal_wndproc_callback_windowed(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam) {
    window_context* context = (window_context*)GetWindowLongPtr(handle, GWLP_USERDATA);
    if (!context) {
        return 0;
    }

    switch (msg) {
        // Handle events when window was:
        //      Requested to close
        //      Requested to minimize
        //      Requested to maximize
        //      Requested to restore from maximize
        //      Requested to restore from taskbar
        //
        // These events override default window behavior.
        case WM_SYSCOMMAND: {
            if (wparam == SC_CLOSE) {
                context->internal_enqueue_event<libgui::ev::ev_close>({});
                return 0;
            }

            if (wparam == SC_MINIMIZE) {
                context->internal_enqueue_event<libgui::ev::ev_minimize>({});
                return 0;
            }

            if (wparam == SC_MAXIMIZE) {
                context->internal_enqueue_event<libgui::ev::ev_maximize>({});
                return 0;
            }

            if (wparam == SC_RESTORE) {
                context->internal_dispatch_event<libgui::internals::ev::ev_restore>({});
                return 0;
            }
            
            break;
        }

        // Handle events when window was:
        //      Minimized/restored to/from taskbar
        //      Maximized/restored
        case WM_SIZE: {
            if (wparam == SIZE_MINIMIZED) {
                if (!context->m_minimized_to_tb) {
                    context->m_minimized_to_tb = true;
                    context->internal_dispatch_event<libgui::ev::ev_minimized>({});
                }
            }
            else if (wparam == SIZE_MAXIMIZED) {
                // If a maximized window is minimized to taskbar and is restored
                // from the taskbar, Win32 API sends a SIZE_MAXIMIZED message.
                //
                // When that happens, send an ev_minimize_restored event since
                // the window was actually restored from taskbar and is already maximized.
                if (context->m_minimized_to_tb) {
                    context->m_minimized_to_tb = false;
                    context->internal_enqueue_event<libgui::ev::ev_minimize_restored>({});
                }
                else if (!context->m_maximized) {
                    context->m_maximized = true;
                    context->internal_enqueue_event<libgui::ev::ev_maximized>({});
                }
            }
            else if (wparam == SIZE_RESTORED) {
                if (context->m_maximized) {
                    context->m_maximized = false;
                    context->internal_enqueue_event<libgui::ev::ev_maximize_restored>({});
                }
                else if (context->m_minimized_to_tb) {
                    context->m_minimized_to_tb = false;
                    context->internal_enqueue_event<libgui::ev::ev_minimize_restored>({});
                }
            }

            break;
        }

        // Handle events when window was:
        //      Minimized/restored to/from system tray
        case WM_SHOWWINDOW: {
            // If call wasn't because of ShowWindow(), do default
            if (lparam != 0)
                break;

            // Window was restored
            if (wparam) {
                context->m_minimized_to_st = false;
                context->internal_enqueue_event<libgui::ev::ev_minimize_restored>({});
            }
            // Window was minimized
            else {
                context->m_minimized_to_st = true;
                context->internal_dispatch_event<libgui::ev::ev_minimized>({});
            }

            break;
        }

        // Handle events when window was:
        //      Requested to restore from system tray
        case WM_USER_TRAYICON: {
            if (lparam == WM_LBUTTONDOWN) {
                context->internal_dispatch_event<libgui::internals::ev::ev_restore>({});
            }

            break;
        }

        default: break;
    }

    return CallWindowProc(context->m_wndproc_default_callback, handle, msg, wparam, lparam);
}

LRESULT window_context::internal_wndproc_callback_borderless(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam) {
    window* wnd = (window*)GetWindowLongPtr(handle, GWLP_USERDATA);
    if (!wnd) {
        return 0;
    }

    window_context* context = wnd->m_context.get();
    if (!context) {
        return 0;
    }

    switch (msg) {
        case WM_NCCALCSIZE: {
            NCCALCSIZE_PARAMS* params = reinterpret_cast<NCCALCSIZE_PARAMS*>(lparam);

            WINDOWPLACEMENT wp = {};
            wp.length          = sizeof(WINDOWPLACEMENT);
            GetWindowPlacement(handle, &wp);

            if (wparam == TRUE && params) {
                if (wp.showCmd == SW_SHOWMAXIMIZED) {
                    params->rgrc[0].top    += 8;
                    params->rgrc[0].right  -= 8;
                    params->rgrc[0].bottom -= 8;
                    params->rgrc[0].left   += 8;
                    return 0;
                }

                params->rgrc[0].top    += 1;
                params->rgrc[0].right  -= 1;
                params->rgrc[0].bottom -= 1;
                params->rgrc[0].left   += 1;
            }

            return 0;
        }
        case WM_NCPAINT: {
            break;
        }
        case WM_NCHITTEST: {
            LRESULT def_result = CallWindowProc(context->m_wndproc_default_callback, handle, msg, wparam, lparam);
            auto    titlebar   = wnd->get_titlebar();
            
            // Handle custom titlebar hit testing
            if (titlebar) {
                POINTS       mouse_pos        = MAKEPOINTS(lparam);
                POINT        client_mouse_pos = { mouse_pos.x, mouse_pos.y };
                ImGuiWindow* modal            = ImGui::GetTopMostAndVisiblePopupModal();

                ScreenToClient(handle, &client_mouse_pos);

                if (titlebar->mouseover_maximize_button)
                    return HTMAXBUTTON;

                if (!modal && !titlebar->m_skip_hit_testing && titlebar->mouseover_titlebar)
                    return HTCAPTION;
            }

            if (!context->m_resizable) {
                switch (def_result) {
                    case HTBOTTOM:
                    case HTBOTTOMLEFT:
                    case HTBOTTOMRIGHT:
                    case HTLEFT:
                    case HTRIGHT:
                    case HTTOP:
                    case HTTOPLEFT:
                    case HTTOPRIGHT:
                        return HTCLIENT;
                    default: break;
                }
            }

            return def_result;
        }
        case WM_NCACTIVATE: {
            return TRUE;
        }
        case WM_GETMINMAXINFO: {
            MINMAXINFO* min_max = (MINMAXINFO*)lparam;

            // Get information about the monitor
            HMONITOR monitor = MonitorFromWindow(handle, MONITOR_DEFAULTTONEAREST);

            MONITORINFO monitor_info = {};
            monitor_info.cbSize      = sizeof(MONITORINFO);
            GetMonitorInfo(monitor, &monitor_info);

            // Set the maximum size based on monitor information
            min_max->ptMaxTrackSize.x = monitor_info.rcWork.right  - monitor_info.rcWork.left;
            min_max->ptMaxTrackSize.y = monitor_info.rcWork.bottom - monitor_info.rcWork.top;
            min_max->ptMaxPosition.x  = 0;
            min_max->ptMaxPosition.y  = 0;

            break;
        }
        case WM_NCLBUTTONDOWN: {
            if (wparam == HTMAXBUTTON)
                return 0;

            if (wparam == HTCAPTION)
                ImGui::ClosePopupsExceptModals();

            break;
        }
        case WM_NCLBUTTONUP: {
            switch (wparam) {
                case HTMAXBUTTON: {
                    context->set_maximized(!context->is_maximized());
                    return 0;
                }

                default: break;
            }

            break;
        }
        case WM_USER_TRAYICON: {
            if (lparam == WM_LBUTTONDOWN) {
                context->internal_restore_from_st();
            }
            break;
        }

        default: break;
    }

    return CallWindowProc(context->m_wndproc_default_callback, handle, msg, wparam, lparam);
}

void window_context::internal_event_callback(const libgui::ev::ev_close& event) {
    if (!m_wnd) return;

    if (m_settings.minimize_to_st_on_close) {
        return set_minimize_to_st(true);
    }

    request_close();
}

void window_context::internal_event_callback(const libgui::ev::ev_minimize& event) {
    if (!m_wnd) return;

    if (m_settings.minimize_to_st_on_minimize) {
        return set_minimize_to_st(true);
    }

    set_minimize_to_tb(true);
}

void window_context::internal_event_callback(const libgui::ev::ev_minimize_restore& event) {
    if (!m_wnd) return;

    if (is_minimized_to_tb()) {
        return set_minimize_to_tb(false);
    }

    if (is_minimized_to_st()) {
        return set_minimize_to_st(false);
    }
}

void window_context::internal_event_callback(const libgui::ev::ev_maximize& event) {
    if (!m_wnd) return;
    set_maximized(true);
}

void window_context::internal_event_callback(const libgui::ev::ev_maximize_restore& event) {
    if (!m_wnd) return;
    set_maximized(false);
}

void window_context::internal_event_callback(const libgui::internals::ev::ev_restore& event) {
    if (!m_wnd || !m_glfw_handle) return;

    if (is_minimized_to_tb() || is_minimized_to_st()) {
        return internal_dispatch_event<libgui::ev::ev_minimize_restore>({});
    }

    if (is_maximized()) {
        return internal_enqueue_event<libgui::ev::ev_maximize_restore>({});
    }

    // Fallback
    glfwRestoreWindow(m_glfw_handle);
}

void window_context::internal_event_callback(const libgui::internals::ev::ev_frame_buffer_resized& event) {
    glViewport(0, 0, event.width, event.height);
}

void window_context::internal_event_callback(const libgui::internals::ev::ev_update_settings& event) {
    m_settings = event.settings;
    
    set_borderless(event.settings.borderless);
    set_resizable(event.settings.resizable);

    // Sync settings with current state

    m_settings.borderless = is_borderless();
    m_settings.resizable  = is_resizable();

    // Fire settings updated event

    libgui::ev::ev_settings_updated ev;
    ev.settings = m_settings;

    internal_enqueue_event(std::move(ev));
}

///////////////////////////////////////////////////////////////////////////////
// GLFW CALLBACKS

void window_context::internal_glfw_window_resize_callback(GLFWwindow* window, int width, int height)
{
    window_context* context = static_cast<window_context*>(glfwGetWindowUserPointer(window));
    if (!context || !context->m_wnd)
        return;

    libgui::ev::ev_resized ev;
    ev.width  = width;
    ev.height = height;

    context->internal_enqueue_event(std::move(ev));
}

void window_context::internal_glfw_frame_buffer_resize_callback(GLFWwindow* window, int width, int height)
{
    window_context* context = static_cast<window_context*>(glfwGetWindowUserPointer(window));
    if (!context || !context->m_wnd)
        return;

    libgui::internals::ev::ev_frame_buffer_resized ev;
    ev.width  = width;
    ev.height = height;

    context->internal_enqueue_event(std::move(ev));
}

void window_context::internal_glfw_scroll_callback(GLFWwindow* window, double dx, double dy)
{
    window_context* context = static_cast<window_context*>(glfwGetWindowUserPointer(window));
    if (!context || !context->m_wnd)
        return;

    libgui::ev::ev_scroll ev;
    ev.dx = dx;
    ev.dy = dy;

    context->internal_enqueue_event(std::move(ev));
}

void window_context::internal_glfw_mouse_btn_callback(GLFWwindow* window, int btn, int state, int mod)
{
    window_context* context = static_cast<window_context*>(glfwGetWindowUserPointer(window));
    if (!context || !context->m_wnd)
        return;

    libgui::ev::ev_mouse_btn_state_updated ev;
    ev.btn   = btn;
    ev.state = state;
    ev.mod   = mod;

    context->internal_enqueue_event(std::move(ev));
}

void window_context::internal_glfw_mouse_move_callback(GLFWwindow* window, double x, double y)
{
    window_context* context = static_cast<window_context*>(glfwGetWindowUserPointer(window));
    if (!context || !context->m_wnd)
        return;

    libgui::ev::ev_mouse_moved ev;
    ev.x = x;
    ev.y = y;

    context->internal_enqueue_event(std::move(ev));
}

void window_context::internal_glfw_drop_callback(GLFWwindow* window, int count, const char* files[])
{
    window_context* context = static_cast<window_context*>(glfwGetWindowUserPointer(window));
    if (!context || !context->m_wnd)
        return;

    libgui::ev::ev_drop ev;
    
    for (int i = 0; i < count; i++) {
        ev.files.push_back(files[i]);
    }

    context->internal_enqueue_event(std::move(ev));
}

///////////////////////////////////////////////////////////////////////////////
// INTERNAL
///////////////////////////////////////////////////////////////////////////////

std::string internal_get_glfw_failure() {
    const char* desc = nullptr;
    return glfwGetError(&desc) != GLFW_NO_ERROR && desc ? std::string(desc) : "";
}
