#include "libgui/window/window_context.hpp"

#include <stdexcept>
#include <versionhelpers.h>

using namespace libgui;
using namespace libgui::internals;

// Message sent when user clicks icon in system tray
#define WM_USER_TRAYICON WM_USER + 1

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

window_context::window_context(window* window, GLFWwindow* handle)
    : m_wnd(window), m_glfw_handle(handle) {}

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
    m_standard_wnd_style = GetWindowLongPtr(win32_handle, GWL_STYLE);

    // Save borderless style
    m_borderless_wnd_style = m_standard_wnd_style;
    m_borderless_wnd_style |= WS_OVERLAPPEDWINDOW;
    m_borderless_wnd_style |= WS_CAPTION;
    m_borderless_wnd_style |= WS_MAXIMIZEBOX;
    m_borderless_wnd_style |= WS_THICKFRAME;
    m_borderless_wnd_style |= CS_VREDRAW;
    m_borderless_wnd_style |= CS_HREDRAW;

    /*
        Set callbacks
    */

    glfwSetWindowSizeCallback(m_glfw_handle, window_context::internal_resize_callback);
    glfwSetScrollCallback(m_glfw_handle, window_context::internal_scroll_callback);
    glfwSetMouseButtonCallback(m_glfw_handle, window_context::internal_mouse_button_callback);
    glfwSetCursorPosCallback(m_glfw_handle, window_context::internal_mouse_move_callback);
    glfwSetWindowIconifyCallback(m_glfw_handle, window_context::internal_minimize_callback);
    glfwSetDropCallback(m_glfw_handle, window_context::internal_drop_callback);

    /*
        Initialize taskbar
    */

    internal_initialize_tb();

    /*
        Initialize system tray
    */

    internal_initialize_st();

    return true;
}

void window_context::teardown() {
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

void window_context::minimize_to_tb() {
    if (!m_glfw_handle)
        return;

    if (is_minimized_to_tb() || is_minimized_to_st())
        return;

    glfwIconifyWindow(m_glfw_handle);
}

void window_context::minimize_to_st() {
    if (!m_glfw_handle)
        return;

    if (is_minimized_to_tb() || is_minimized_to_st())
        return;

    auto win32_handle = get_win32_handle();
    if (!win32_handle)
        return;

    if (!internal_set_st_icon_visible(true)) {
        return;
    }

    if (!ShowWindow(win32_handle, SW_HIDE)) {
        internal_set_st_icon_visible(false);
        return;
    }

    m_minimized_to_st = true;
    window_context::internal_minimize_callback(m_glfw_handle, GLFW_TRUE);
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
        SetWindowLongPtr(win32_handle, GWL_STYLE,    m_standard_wnd_style);
        SetWindowLongPtr(win32_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(internals::window_context::internal_wndproc_callback_standard));

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
    if (!m_glfw_handle) return false;
    return glfwGetWindowAttrib(m_glfw_handle, GLFW_ICONIFIED) && !m_minimized_to_st;
}

bool window_context::is_minimized_to_st() const {
    return m_minimized_to_st;
}

bool window_context::is_maximized() const {
    if (!m_glfw_handle) return false;
    return glfwGetWindowAttrib(m_glfw_handle, GLFW_MAXIMIZED);
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
    if (m_st_handle || !m_glfw_handle)
        return;

    auto window = static_cast<libgui::window*>(glfwGetWindowUserPointer(m_glfw_handle));
    if (!window)
        return;

    auto win32_handle = glfwGetWin32Window(m_glfw_handle);
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

void window_context::internal_restore_from_st() {
    if (!m_glfw_handle)
        return;

    auto window = static_cast<libgui::window*>(glfwGetWindowUserPointer(m_glfw_handle));
    if (!window)
        return;

    auto win32_handle = glfwGetWin32Window(m_glfw_handle);
    if (!win32_handle) 
        return;

    if (!internal_set_st_icon_visible(false))
        return;

    if (ShowWindow(win32_handle, SW_SHOW)) {
        internal_set_st_icon_visible(true);
        return;
    }

    SetForegroundWindow(win32_handle);

    m_minimized_to_st = false;
    window_context::internal_minimize_callback(m_glfw_handle, GLFW_FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// WNDPROC

LRESULT window_context::internal_wndproc_callback_standard(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam) {
    window* wnd = (window*)GetWindowLongPtr(handle, GWLP_USERDATA);
    if (!wnd) {
        return 0;
    }

    window_context* context = wnd->m_context.get();
    if (!context) {
        return 0;
    }
    
    switch (msg) {
        case WM_USER_TRAYICON: {
            if (lparam == WM_LBUTTONDOWN) {
                context->set_maximized(!context->is_maximized());
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

///////////////////////////////////////////////////////////////////////////////
// GLFW CALLBACKS

void window_context::internal_resize_callback(GLFWwindow* window, int width, int height) {
    if (width == 0 || height == 0) return;

    libgui::window* instance = static_cast<libgui::window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    glViewport(0, 0, width, height);
    instance->on_resize(width, height);
}

void window_context::internal_scroll_callback(GLFWwindow* window, double dx, double dy) {
    libgui::window* instance = static_cast<libgui::window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    instance->on_scroll(dx, dy);
}

void window_context::internal_mouse_button_callback(GLFWwindow* window, int button, int action, int modifier) {
    libgui::window* instance = static_cast<libgui::window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    instance->on_mouse_button(button, action, modifier);
}

void window_context::internal_mouse_move_callback(GLFWwindow* window, double x, double y) {
    libgui::window* instance = static_cast<libgui::window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    instance->m_mouse_pos.dx = x - instance->m_mouse_pos.x;
    instance->m_mouse_pos.dy = instance->m_mouse_pos.y - y;
    instance->m_mouse_pos.x  = x;
    instance->m_mouse_pos.y  = y;

    instance->on_mouse_move(instance->m_mouse_pos.dx, instance->m_mouse_pos.dy);
}

void window_context::internal_minimize_callback(GLFWwindow* window, int minimized) {
    libgui::window* instance = static_cast<libgui::window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    instance->on_minimize(minimized);
}

void window_context::internal_drop_callback(GLFWwindow* window, int count, const char* paths[]) {
    libgui::window* instance = static_cast<libgui::window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    instance->on_drop(count, paths);
}