#include "libgui/internals/defines/windowing.hpp"
#include "libgui/internals/utilities/glfw.hpp"
#include "libgui/internals/utilities/win32.hpp"
#include "libgui/internals/window/window_titlebar.hpp"
#include "libgui/utilities/string.hpp"
#include "libgui/window/contexts/windowing_context_glfw.hpp"
#include "libgui/window/window_base.hpp"
#include "libgui/window/contexts/backend_context.hpp"

#include <versionhelpers.h>

using namespace libgui::internals;

// Message sent when user clicks icon in system tray
#define WM_USER_TRAYICON WM_USER + 1

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

libgui::result windowing_context_glfw::initialize(const window_settings& settings,
    const window_startup_settings& startup_settings)
{
    libgui::result result, res;

    // Cache settings
    m_settings = settings;
    
    res = internal_initialize_glfw();
    if (!res) {
        result.message = LIBGUI_FORMAT("Failed to initialize windowing context. | {}", res.message);
        return result;
    }

    res = internal_initialize_win32();
    if (!res) {
        result.message = LIBGUI_FORMAT("Failed to initialize windowing context. | {}", res.message);
        return result;
    }

    // Set event handlers
    internal_initialize_event_handlers();

    // Initialize taskbar
    internal_initialize_tb();

    // Initialize system tray
    internal_initialize_st();

    // Apply startup settings
    {
        internals::ev::ev_update_startup_settings ev;
        ev.settings = startup_settings;

        internal_event_enqueue(std::move(ev));
    }

    // Apply standard settings
    set_settings(m_settings);

    result.success = true;
    return result;
}

void windowing_context_glfw::teardown() {
    internal_teardown_st();
    internal_teardown_tb();
    internal_teardown_event_handlers();
    internal_teardown_win32();
    internal_teardown_glfw();
}

void windowing_context_glfw::set_settings(const window_settings& settings) {
    libgui::internals::ev::ev_update_settings ev;
    ev.settings = settings;

    internal_event_enqueue(std::move(ev));
}

void windowing_context_glfw::event_poll() {
    glfwPollEvents();
    
    if (m_wnd_internal) {
        m_wnd_internal->m_event_system.process();
    }
}

void windowing_context_glfw::event_wait() {
    glfwWaitEvents();
}

void windowing_context_glfw::set_tb_state(taskbar_state state) {
    if (!m_glfw_handle || !m_tb_handle) {
        return;
    }

    auto win32_handle = get_win32_handle();
    if (!win32_handle) {
        return;
    }

    m_tb_handle->SetProgressState(win32_handle, (TBPFLAG)state);
}

void windowing_context_glfw::set_tb_value(uint64_t value) {
    if (!m_glfw_handle || !m_tb_handle) {
        return;
    }

    auto win32_handle = get_win32_handle();
    if (!win32_handle) {
        return;
    }

    m_tb_handle->SetProgressValue(win32_handle, (ULONGLONG)max(value, 100), 100);
}

bool windowing_context_glfw::is_closing() const {
    if (!m_glfw_handle) {
        return true;
    }

    return glfwWindowShouldClose(m_glfw_handle);
}

void windowing_context_glfw::prepare_for_drawing() {
    if (!m_glfw_handle || !m_wnd_internal || !m_wnd_internal->m_backend_context) {
        return;
    }

    m_wnd_internal->m_backend_context->prepare_for_drawing();
}

void windowing_context_glfw::draw() {
    if (!m_glfw_handle || !m_wnd_internal || !m_wnd_internal->m_backend_context) {
        return;
    }

    m_wnd_internal->m_backend_context->draw();
    glfwSwapBuffers(m_glfw_handle);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE
///////////////////////////////////////////////////////////////////////////////

HWND windowing_context_glfw::get_win32_handle() {
    if (!m_glfw_handle) {
        return nullptr;
    }

    return glfwGetWin32Window(m_glfw_handle);
}

libgui::result windowing_context_glfw::internal_initialize_glfw() {
    libgui::result result;

    m_glfw_handle = glfwCreateWindow(1, 1, m_settings.title.c_str(), NULL, NULL);
    if (!m_glfw_handle) {
        result.message = LIBGUI_FORMAT("Failed to create window. | {}", glfw::get_failure_desc());
        return result;
    }

    auto win32_handle = get_win32_handle();
    if (!win32_handle) {
        result.message = "Failed to create window.";
        return result;
    }

    glfwMakeContextCurrent(m_glfw_handle);

    SetWindowLongPtr(win32_handle, GWLP_USERDATA, (intptr_t)this);
    glfwSetWindowUserPointer(m_glfw_handle, this);

    glfwSetWindowSizeCallback(m_glfw_handle, windowing_context_glfw::internal_glfw_window_resize_callback);
    glfwSetFramebufferSizeCallback(m_glfw_handle, windowing_context_glfw::internal_glfw_frame_buffer_resize_callback);
    glfwSetScrollCallback(m_glfw_handle, windowing_context_glfw::internal_glfw_scroll_callback);
    glfwSetMouseButtonCallback(m_glfw_handle, windowing_context_glfw::internal_glfw_mouse_btn_callback);
    glfwSetCursorPosCallback(m_glfw_handle, windowing_context_glfw::internal_glfw_mouse_move_callback);
    glfwSetDropCallback(m_glfw_handle, windowing_context_glfw::internal_glfw_drop_callback);

    result.success = true;
    return result;
}

void windowing_context_glfw::internal_teardown_glfw() {

}

libgui::result windowing_context_glfw::internal_initialize_win32() {
    libgui::result result;

    auto win32_handle = get_win32_handle();
    if (!win32_handle) {
        result.message = "Window is null.";
        return result;
    }

    // Save default callback
    m_default_wndproc_callback = (WNDPROC)GetWindowLongPtr(win32_handle, GWLP_WNDPROC);
    if (!m_default_wndproc_callback) {
        result.message = "Failed to get default WNDPROC.";
        return result;
    }

    // Configure Win32 style parameters
    auto win32_style = GetWindowLongPtr(win32_handle, GWL_STYLE);
    LIBGUI_SET_FLAG(win32_style, WS_OVERLAPPEDWINDOW, true);

    // Change Win32 parameters
    SetWindowLongPtr(win32_handle, GWL_STYLE,    win32_style);
    SetWindowLongPtr(win32_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(windowing_context_glfw::internal_wndproc_callback));

    result.success = true;
    return result;
}

void windowing_context_glfw::internal_teardown_win32() {

}

void windowing_context_glfw::internal_initialize_event_handlers() {

}

void windowing_context_glfw::internal_teardown_event_handlers() {

}

void windowing_context_glfw::internal_initialize_tb() {
    if (!IsWindows7OrGreater() || m_tb_handle) {
        return;
    }

    HRESULT result = CoInitialize(NULL);
    if ((result != S_OK) && (result != S_FALSE)) {
        return;
    }

    result = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_tb_handle));
    if (!SUCCEEDED(result)) {
        return internal_teardown_tb();
    }

    result = m_tb_handle->HrInit();
    if (!SUCCEEDED(result)) {
        return internal_teardown_tb();
    }
}

void windowing_context_glfw::internal_teardown_tb() {
    if (!m_tb_handle) {
        return;
    }

    m_tb_handle->Release();
    m_tb_handle = nullptr;
}

void windowing_context_glfw::internal_initialize_st() {
    if (m_st_handle || !m_glfw_handle) {
        return;
    }

    auto win32_handle = get_win32_handle();
    if (!win32_handle) {
        return;
    }

    m_st_handle                   = new NOTIFYICONDATA();
    m_st_handle->cbSize           = sizeof(NOTIFYICONDATA);
    m_st_handle->hWnd             = win32_handle;
    m_st_handle->uID              = 100;
    m_st_handle->uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    m_st_handle->uCallbackMessage = WM_USER_TRAYICON;
    m_st_handle->hIcon            = (HICON)GetClassLongPtr(win32_handle, GCLP_HICON);

    strncpy(m_st_handle->szTip, m_settings.title.c_str(),
        min(sizeof(m_st_handle->szTip), m_settings.title.size()));
}

void windowing_context_glfw::internal_teardown_st() {
    if (!m_st_handle) {
        return;
    }

    internal_set_st_icon_visible(false);

    delete m_st_handle;
    m_st_handle = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
// WIN32 EVENTS

bool windowing_context_glfw::internal_set_st_icon_visible(bool value) {
    if (!m_st_handle)
        return false;

    bool st_icon_visible = LIBGUI_HAS_FLAG(m_state, LIBGUI_WND_STATE_ST_ICON_VISIBLE);

    if (!st_icon_visible && value) {
        Shell_NotifyIcon(NIM_ADD, m_st_handle);
    }
    else if (st_icon_visible && !value) {
        Shell_NotifyIcon(NIM_DELETE, m_st_handle);
    }
    else {
        return false;
    }

    LIBGUI_SET_FLAG(m_state, LIBGUI_WND_STATE_ST_ICON_VISIBLE, value);
    return true;
}

LRESULT __stdcall windowing_context_glfw::internal_wndproc_callback(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    windowing_context_glfw* context = (windowing_context_glfw*)GetWindowLongPtr(handle, GWLP_USERDATA);
    if (!context || !context->m_wnd || !context->m_wnd_internal) {
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
        case WM_SYSCOMMAND: return context->internal_wndproc_wmsyscommand(handle, msg, wparam, lparam);

        // Handle events when window was:
        //      Minimized/restored to/from taskbar
        //      Maximized/restored
        case WM_SIZE: return context->internal_wndproc_wmsize(handle, msg, wparam, lparam);

        // Handle events when window was:
        //      Minimized/restored to/from system tray
        case WM_SHOWWINDOW: return context->internal_wndproc_wmshowwindow(handle, msg, wparam, lparam);

        // Handle events when window was:
        //      Requested to restore from system tray
        case WM_USER_TRAYICON: return context->internal_wndproc_wmusertrayicon(handle, msg, wparam, lparam);

        // Handle NC frame size
        case WM_NCCALCSIZE: return context->internal_wndproc_wmnccalcsize(handle, msg, wparam, lparam);

        // Handle custom titlebar hit testing
        // Handle resize area hit testing
        case WM_NCHITTEST: return context->internal_wndproc_wmnchittest(handle, msg, wparam, lparam);

        // Handle size constraints
        // Handle maximize size
        case WM_GETMINMAXINFO: return context->internal_wndproc_wmgetminmaxinfo(handle, msg, wparam, lparam);

        // Handle close, maximize, minimize button down events
        case WM_NCLBUTTONDOWN: return context->internal_wndproc_wmnclbuttondown(handle, msg, wparam, lparam);

        // Handle close, maximize, minimize button up events
        case WM_NCLBUTTONUP: return context->internal_wndproc_wmnclbuttonup(handle, msg, wparam, lparam);

        default: break;
    }

    return CallWindowProc(context->m_default_wndproc_callback, handle, msg, wparam, lparam);
}

LRESULT windowing_context_glfw::internal_wndproc_wmsyscommand(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Handle close command
    if (wparam == SC_CLOSE) {
        internal_event_enqueue<libgui::ev::ev_close>({});
        return 0;
    }

    // Handle maximize command
    if (wparam == SC_MAXIMIZE) {
        internal_event_enqueue<libgui::ev::ev_maximize>({});
        return 0;
    }

    // Handle minimize command
    if (wparam == SC_MINIMIZE) {
        internal_event_enqueue<libgui::ev::ev_minimize_to_tb>({});
        return 0;
    }

    // Handle restore from maximize/taskbar command
    if (wparam == SC_RESTORE) {
        windowing_context_glfw* context = (windowing_context_glfw*)GetWindowLongPtr(handle, GWLP_USERDATA);
        if (!context) {
            return 0;
        }

        if (context->is_maximized()) {
            internal_event_enqueue<libgui::ev::ev_maximize_restore>({});
            return 0;
        }

        if (context->is_minimized_to_tb()) {
            internal_event_dispatch<libgui::ev::ev_minimize_restore_from_tb>({});
            return 0;
        }
    }

    return CallWindowProc(m_default_wndproc_callback, handle, msg, wparam, lparam);
}

LRESULT windowing_context_glfw::internal_wndproc_wmsize(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (wparam == SIZE_MINIMIZED) {
        if (!is_minimized_to_tb()) {
            LIBGUI_SET_FLAG(m_state, LIBGUI_WND_STATE_MINIMIZED_TO_TB, true);
            internal_event_dispatch<libgui::ev::ev_minimized_to_tb>({});
        }
    }
    else if (wparam == SIZE_MAXIMIZED) {
        // If a maximized window is minimized to taskbar and is restored
        // from the taskbar, Win32 API sends a SIZE_MAXIMIZED message.
        //
        // When that happens, send an ev_minimize_restored event since
        // the window was actually restored from taskbar and is already maximized.
        if (is_minimized_to_tb()) {
            LIBGUI_SET_FLAG(m_state, LIBGUI_WND_STATE_MINIMIZED_TO_TB, false);
            internal_event_enqueue<libgui::ev::ev_minimize_restored_from_tb>({});
        }
        else if (!is_maximized()) {
            LIBGUI_SET_FLAG(m_state, LIBGUI_WND_STATE_MAXIMIZED, true);
            internal_event_enqueue<libgui::ev::ev_maximized>({});
        }
    }
    else if (wparam == SIZE_RESTORED) {
        if (is_maximized()) {
            LIBGUI_SET_FLAG(m_state, LIBGUI_WND_STATE_MAXIMIZED, false);
            internal_event_enqueue<libgui::ev::ev_maximize_restored>({});
        }
        else if (is_minimized_to_tb()) {
            LIBGUI_SET_FLAG(m_state, LIBGUI_WND_STATE_MINIMIZED_TO_TB, false);
            internal_event_enqueue<libgui::ev::ev_minimize_restored_from_tb>({});
        }
    }

    return CallWindowProc(m_default_wndproc_callback, handle, msg, wparam, lparam);
}

LRESULT windowing_context_glfw::internal_wndproc_wmshowwindow(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    // If call was because of ShowWindow()
    if (!lparam) {
        // Window was restored
        if (wparam) {
            LIBGUI_SET_FLAG(m_state, LIBGUI_WND_STATE_MINIMIZED_TO_ST, false);
            internal_event_enqueue<libgui::ev::ev_minimize_restored_from_st>({});
        }
        // Window was minimized
        else {
            LIBGUI_SET_FLAG(m_state, LIBGUI_WND_STATE_MINIMIZED_TO_ST, true);
            internal_event_dispatch<libgui::ev::ev_minimized_to_st>({});
        }
    }

    return CallWindowProc(m_default_wndproc_callback, handle, msg, wparam, lparam);
}

LRESULT windowing_context_glfw::internal_wndproc_wmusertrayicon(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (lparam == WM_LBUTTONDOWN) {
        internal_event_dispatch<libgui::ev::ev_minimize_restore_from_st>({});
    }

    return CallWindowProc(m_default_wndproc_callback, handle, msg, wparam, lparam);
}

LRESULT windowing_context_glfw::internal_wndproc_wmnccalcsize(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (!wparam) {
        return 0;
    }

    NCCALCSIZE_PARAMS* params = reinterpret_cast<NCCALCSIZE_PARAMS*>(lparam);
    if (!params) {
        return CallWindowProc(m_default_wndproc_callback, handle, msg, wparam, lparam);
    }

    WINDOWPLACEMENT wp = {};
    wp.length = sizeof(WINDOWPLACEMENT);

    if (!GetWindowPlacement(handle, &wp)) {
        return CallWindowProc(m_default_wndproc_callback, handle, msg, wparam, lparam);
    }

    auto nc_size = win32::get_window_nc_size(handle, wp.showCmd == SW_MAXIMIZE);

    params->rgrc[0].top    += nc_size.top;
    params->rgrc[0].right  -= nc_size.right;
    params->rgrc[0].bottom -= nc_size.bottom;
    params->rgrc[0].left   += nc_size.left;

    return 0;
}

LRESULT windowing_context_glfw::internal_wndproc_wmnchittest(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT def_result = CallWindowProc(m_default_wndproc_callback, handle, msg, wparam, lparam);
    auto    flags      = m_wnd->tb_flags;

    // Title bar hit testing

    if (LIBGUI_HAS_FLAG(flags, LIBGUI_WTB_MO_MINIMIZE))
        return HTMINBUTTON;

    if (LIBGUI_HAS_FLAG(flags, LIBGUI_WTB_MO_MAXIMIZE))
        return HTMAXBUTTON;

    if (LIBGUI_HAS_FLAG(flags, LIBGUI_WTB_MO_CLOSE))
        return HTCLOSE;

    if (!LIBGUI_HAS_FLAG(flags, LIBGUI_WTB_SKIP) && LIBGUI_HAS_FLAG(flags, LIBGUI_WTB_MO_TB))
        return HTCAPTION;

    // Disable resize areas if window is not resizable
    if (!is_resizable()) {
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

LRESULT windowing_context_glfw::internal_wndproc_wmgetminmaxinfo(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Get default values
    auto def_result = CallWindowProc(m_default_wndproc_callback, handle, msg, wparam, lparam);

    MINMAXINFO* minmax = (MINMAXINFO*)lparam;
    if (!minmax) {
        return def_result;
    }

    // Get PRIMARY monitor
    // PRIMARY monitor's top left is always at 0,0
    auto mon_primary = MonitorFromPoint({ 0, 0 }, MONITOR_DEFAULTTOPRIMARY);

    // Get CURRENT monitor
    auto mon_current = MonitorFromWindow(handle, MONITOR_DEFAULTTONEAREST);

    if (!mon_primary || !mon_current) {
        return def_result;
    }

    // Get monitor infos

    MONITORINFO mon_info_primary = {};
    MONITORINFO mon_info_current = {};
    mon_info_primary.cbSize = sizeof(MONITORINFO);
    mon_info_current.cbSize = sizeof(MONITORINFO);

    if (!GetMonitorInfo(mon_primary, &mon_info_primary)) {
        return def_result;
    }

    if (!GetMonitorInfo(mon_current, &mon_info_current)) {
        return def_result;
    }

    // Get NC frame size
    auto nc_frame = win32::get_window_nc_size(handle, true);

    // Set max position so that the NC frame is not visible
    minmax->ptMaxPosition.x = -nc_frame.left;
    minmax->ptMaxPosition.y = -nc_frame.top;

    // Max size is set based on the PRIMARY monitor
    // Resizing for other monitors is done automatically by the window manager
    // Extend max size so that client area fills the screen

    // Extend by left and right NC size
    minmax->ptMaxSize.x = mon_info_primary.rcWork.right + nc_frame.left + nc_frame.right;
    // Extend by top and bottom NC size
    minmax->ptMaxSize.y = mon_info_primary.rcWork.bottom + nc_frame.top + nc_frame.bottom;

    // Max manual resize size is set based on the CURRENT monitor

    // Extend by left and right NC size
    minmax->ptMaxTrackSize.x = mon_info_current.rcWork.right + nc_frame.left + nc_frame.right;
    // Extend by top and bottom NC size
    minmax->ptMaxTrackSize.y = mon_info_current.rcWork.bottom + nc_frame.top + nc_frame.bottom;

    /*minmax->ptMinTrackSize.x = context->m_settings.min_width;
    minmax->ptMinTrackSize.y = context->m_settings.min_height;*/

    return 0;
}

LRESULT windowing_context_glfw::internal_wndproc_wmnclbuttondown(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    auto& flags = m_wnd->tb_flags;

    if (wparam == HTMINBUTTON) {
        LIBGUI_SET_FLAG(flags, LIBGUI_WTB_LDOWN_MINIMIZE, true);
        return 0;
    }

    if (wparam == HTMAXBUTTON) {
        LIBGUI_SET_FLAG(flags, LIBGUI_WTB_LDOWN_MAXIMIZE, true);
        return 0;
    }

    if (wparam == HTCLOSE) {
        LIBGUI_SET_FLAG(flags, LIBGUI_WTB_LDOWN_CLOSE, true);
        return 0;
    }

    if (wparam == HTCAPTION) {
        ImGui::ClosePopupsExceptModals();
    }

    return CallWindowProc(m_default_wndproc_callback, handle, msg, wparam, lparam);
}

LRESULT windowing_context_glfw::internal_wndproc_wmnclbuttonup(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    auto& flags = m_wnd->tb_flags;

    if (wparam == HTMINBUTTON) {
        if (LIBGUI_HAS_FLAG(flags, LIBGUI_WTB_LDOWN_MINIMIZE)) {
            LIBGUI_SET_FLAG(flags, LIBGUI_WTB_LDOWN_MINIMIZE, false);

            internal_event_enqueue<libgui::ev::ev_minimize_to_tb>({});
        }

        return 0;
    }

    if (wparam == HTMAXBUTTON) {
        if (LIBGUI_HAS_FLAG(flags, LIBGUI_WTB_LDOWN_MAXIMIZE)) {
            LIBGUI_SET_FLAG(flags, LIBGUI_WTB_LDOWN_MAXIMIZE, false);

            if (!is_maximized()) {
                internal_event_enqueue<libgui::ev::ev_maximize>({});
            }
            else {
                internal_event_enqueue<libgui::ev::ev_maximize_restore>({});
            }
        }

        return 0;
    }

    if (wparam == HTCLOSE) {
        if (LIBGUI_HAS_FLAG(flags, LIBGUI_WTB_LDOWN_CLOSE)) {
            LIBGUI_SET_FLAG(flags, LIBGUI_WTB_LDOWN_CLOSE, false);

            internal_event_enqueue<libgui::ev::ev_close>({});
        }

        return 0;
    }

    LIBGUI_SET_FLAG(flags, LIBGUI_WTB_LDOWN_ALL, false);
    return CallWindowProc(m_default_wndproc_callback, handle, msg, wparam, lparam);
}

///////////////////////////////////////////////////////////////////////////////
// REQUEST EVENTS

void windowing_context_glfw::internal_event_callback(const internals::ev::ev_close& event) {
    if (!m_glfw_handle) {
        return;
    }

    if (event.force) {
        return glfwSetWindowShouldClose(m_glfw_handle, GLFW_TRUE);
    }

    internal_event_dispatch<libgui::ev::ev_close>({});
}

///////////////////////////////////////////////////////////////////////////////
// BEHAVIOR EVENTS

void windowing_context_glfw::internal_event_callback(const libgui::ev::ev_close& event) {
    if (!m_glfw_handle) {
        return;
    }

    if (m_settings.minimize_to_st_on_close) {
        return internal_event_dispatch<libgui::ev::ev_minimize_to_st>({});
    }

    glfwSetWindowShouldClose(m_glfw_handle, GLFW_TRUE);
}

void windowing_context_glfw::internal_event_callback(const libgui::ev::ev_maximize& event) {
    if (!m_glfw_handle) {
        return;
    }

    if (is_maximized() || !is_resizable()) {
        return;
    }

    glfwMaximizeWindow(m_glfw_handle);
}

void windowing_context_glfw::internal_event_callback(const libgui::ev::ev_minimize_to_tb& event) {
    if (!m_glfw_handle) {
        return;
    }

    if (is_minimized_to_tb() || is_minimized_to_st()) {
        return;
    }

    glfwIconifyWindow(m_glfw_handle);
}

void windowing_context_glfw::internal_event_callback(const libgui::ev::ev_minimize_to_st& event) {
    if (!m_glfw_handle) {
        return;
    }

    if (is_minimized_to_tb() || is_minimized_to_st()) {
        return;
    }

    auto win32_handle = get_win32_handle();
    if (!win32_handle) {
        return;
    }

    if (!internal_set_st_icon_visible(true)) {
        return;
    }

    if (!ShowWindow(win32_handle, SW_HIDE)) {
        internal_set_st_icon_visible(false);
        return;
    }
}

void windowing_context_glfw::internal_event_callback(const libgui::ev::ev_maximize_restore& event) {
    if (!m_glfw_handle) {
        return;
    }

    if (!is_maximized() || !is_resizable()) {
        return;
    }

    glfwRestoreWindow(m_glfw_handle);
}

void windowing_context_glfw::internal_event_callback(const libgui::ev::ev_minimize_restore_from_tb& event) {
    if (!m_glfw_handle) {
        return;
    }

    if (!is_minimized_to_tb()) {
        return;
    }

    glfwRestoreWindow(m_glfw_handle);
}

void windowing_context_glfw::internal_event_callback(const libgui::ev::ev_minimize_restore_from_st& event) {
    if (!m_glfw_handle) {
        return;
    }

    if (!is_minimized_to_st()) {
        return;
    }

    auto win32_handle = get_win32_handle();
    if (!win32_handle) {
        return;
    }

    if (!internal_set_st_icon_visible(false)) {
        return;
    }

    if (ShowWindow(win32_handle, SW_SHOW)) {
        internal_set_st_icon_visible(true);
        return;
    }

    SetForegroundWindow(win32_handle);
}

void windowing_context_glfw::internal_event_callback(const internals::ev::ev_frame_buffer_resized& event) {
    if (!m_wnd_internal || !m_wnd_internal->m_backend_context) {
        return;
    }

    m_wnd_internal->m_backend_context->set_viewport(0, 0, event.width, event.height);
}

void windowing_context_glfw::internal_event_callback(const internals::ev::ev_update_settings& event) {
    m_settings = event.settings;

    // TODO: Update title

    // Fire settings updated event

    libgui::ev::ev_settings_updated ev;
    ev.settings = m_settings;

    internal_event_enqueue(std::move(ev));
}

void windowing_context_glfw::internal_event_callback(const internals::ev::ev_update_startup_settings& event) {
    auto win32_handle = get_win32_handle();
    if (!win32_handle) {
        return;
    }

    auto dpi = win32::get_window_dpi_scaling(win32_handle);
    auto nc  = win32::get_window_nc_size(win32_handle, false);

    // Convert client area sizes to window sizes.
    // Window sizes contain the NC frame.

    libgui::size_i size         = event.settings.size;
    libgui::size_i minimum_size = event.settings.minimum_size;

    size.scale(dpi);

    size.width  += nc.left + nc.right;
    size.height += nc.top + nc.bottom;

    if (minimum_size.width > 0) {
        minimum_size.width  = static_cast<int32_t>(minimum_size.width * dpi);
        minimum_size.width += nc.left + nc.right;

        size.width = max(size.width, minimum_size.width);
    }

    if (minimum_size.height > 0) {
        minimum_size.height = static_cast<int32_t>(minimum_size.height * dpi);
        minimum_size.height += nc.top + nc.bottom;

        size.height = max(size.height, minimum_size.height);
    }

    // Set window size
    win32::set_window_size(win32_handle, size);

    // Handle size constraints and resizable
    if (event.settings.resizable) {
        m_minimum_size.width  = minimum_size.width;
        m_minimum_size.height = minimum_size.height;
        m_maximum_size.width  = -1;
        m_maximum_size.height = -1;
    }
    else {
        m_minimum_size.width  = size.width;
        m_minimum_size.height = size.height;
        m_maximum_size.width  = size.width;
        m_maximum_size.height = size.height;
    }

    LIBGUI_SET_FLAG(m_state, LIBGUI_WND_STATE_RESIZABLE, event.settings.resizable);

    // Handle maximized
    if (event.settings.maximized) {
        set_maximized(true);
    }

    // Handle centered
    if (event.settings.centered && !is_maximized()) {
        win32::set_window_pos_relative(win32_handle, { 0.5f, 0.5f }, { 0.5f, 0.5f });
    }

    // Handle minimized to system tray
    if (event.settings.minimized_to_st) {
        set_minimize_to_st(true);
    }
}

///////////////////////////////////////////////////////////////////////////////
// NOTIFY EVENTS

///////////////////////////////////////////////////////////////////////////////
// GLFW EVENTS

void windowing_context_glfw::internal_glfw_window_resize_callback(GLFWwindow* window, int width, int height)
{
    windowing_context_glfw* context = static_cast<windowing_context_glfw*>(glfwGetWindowUserPointer(window));
    if (!context) {
        return;
    }

    libgui::ev::ev_resized ev;
    ev.width  = width;
    ev.height = height;

    context->internal_event_enqueue(std::move(ev));
}

void windowing_context_glfw::internal_glfw_frame_buffer_resize_callback(GLFWwindow* window, int width, int height)
{
    windowing_context_glfw* context = static_cast<windowing_context_glfw*>(glfwGetWindowUserPointer(window));
    if (!context) {
        return;
    }

    libgui::internals::ev::ev_frame_buffer_resized ev;
    ev.width  = width;
    ev.height = height;

    context->internal_event_enqueue(std::move(ev));
}

void windowing_context_glfw::internal_glfw_scroll_callback(GLFWwindow* window, double dx, double dy)
{
    windowing_context_glfw* context = static_cast<windowing_context_glfw*>(glfwGetWindowUserPointer(window));
    if (!context) {
        return;
    }

    libgui::ev::ev_scroll ev;
    ev.dx = dx;
    ev.dy = dy;

    context->internal_event_enqueue(std::move(ev));
}

void windowing_context_glfw::internal_glfw_mouse_btn_callback(GLFWwindow* window, int btn, int state, int mod)
{
    windowing_context_glfw* context = static_cast<windowing_context_glfw*>(glfwGetWindowUserPointer(window));
    if (!context) {
        return;
    }

    libgui::ev::ev_mouse_btn_state_updated ev;
    ev.btn   = btn;
    ev.state = state;
    ev.mod   = mod;

    context->internal_event_enqueue(std::move(ev));
}

void windowing_context_glfw::internal_glfw_mouse_move_callback(GLFWwindow* window, double x, double y)
{
    windowing_context_glfw* context = static_cast<windowing_context_glfw*>(glfwGetWindowUserPointer(window));
    if (!context) {
        return;
    }

    libgui::ev::ev_mouse_moved ev;
    ev.x = x;
    ev.y = y;

    context->internal_event_enqueue(std::move(ev));
}

void windowing_context_glfw::internal_glfw_drop_callback(GLFWwindow* window, int count, const char* files[])
{
    windowing_context_glfw* context = static_cast<windowing_context_glfw*>(glfwGetWindowUserPointer(window));
    if (!context) {
        return;
    }

    libgui::ev::ev_drop ev;

    for (int i = 0; i < count; i++) {
        ev.files.push_back(files[i]);
    }

    context->internal_event_enqueue(std::move(ev));
}
