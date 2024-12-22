#define STRICT_TYPED_ITEMIDS

#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <stdexcept>
#include <versionhelpers.h>

#include "libgui/window.hpp"
#include "libgui/imgui.hpp"
#include "libgui/theme.hpp"
#include "libgui/utilities/util_debug.hpp"
#include "libutil/log.hpp"

#ifdef LIBGUI_OPENGL2
    #include <imgui/backends/imgui_impl_opengl2.h>
    #include <imgui/backends/imgui_impl_glfw.h>

    #define LIBGUI_IMGUI_OPENGL_INIT      ImGui_ImplOpenGL2_Init
    #define LIBGUI_IMGUI_OPENGL_SHUTDOWN  ImGui_ImplOpenGL2_Shutdown
    #define LIBGUI_IMGUI_OPENGL_NEW_FRAME ImGui_ImplOpenGL2_NewFrame
    #define LIBGUI_IMGUI_OPENGL_RENDER    ImGui_ImplOpenGL2_RenderDrawData
#elif  LIBGUI_OPENGL3
    #include <glad/glad.h>
    #include <imgui/backends/imgui_impl_opengl3.h>
    #include <imgui/backends/imgui_impl_glfw.h>

    #define LIBGUI_IMGUI_OPENGL_INIT      ImGui_ImplOpenGL3_Init
    #define LIBGUI_IMGUI_OPENGL_SHUTDOWN  ImGui_ImplOpenGL3_Shutdown
    #define LIBGUI_IMGUI_OPENGL_NEW_FRAME ImGui_ImplOpenGL3_NewFrame
    #define LIBGUI_IMGUI_OPENGL_RENDER    ImGui_ImplOpenGL3_RenderDrawData
#else
    #error Unsupported backend
#endif

#define LIBGUI_NATIVE         GLFWwindow*
#define LIBGUI_TO_NATIVE(ptr) (LIBGUI_NATIVE)ptr
#define WM_USER_TRAYICON      WM_USER + 1

#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>

using namespace libgui;

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

namespace libgui {
    class window_context {
    public:
        static LRESULT CALLBACK wndproc_callback(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

        static void resize_callback(GLFWwindow* window, int width, int height);
        static void scroll_callback(GLFWwindow* window, double dx, double dy);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int modifier);
        static void mouse_move_callback(GLFWwindow* window, double x, double y);
        static void iconify_callback(GLFWwindow* window, int iconified);
        static void drop_callback(GLFWwindow* window, int count, const char* paths[]);
    };
}

static constexpr auto get_imgui_version() {
    auto ver = std::string(ImGui::GetVersion());

#ifdef IMGUI_HAS_DOCK
    ver += " docking";
#endif

#ifdef IMGUI_HAS_VIEWPORT
    ver += " multi-viewport";
#endif

    return ver;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

window::window(const window_settings& settings)
    : m_settings(settings)
{
    m_native = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), NULL, NULL);
    if (!m_native) {
        throw std::runtime_error("Failed to create window.");
    }

    auto glfw_handle  = (GLFWwindow*)m_native;
    auto win32_handle = glfwGetWin32Window(glfw_handle);

    if (!win32_handle) {
        throw std::runtime_error("Failed to create window.");
    }


    glfwMakeContextCurrent(glfw_handle);
    glfwSwapInterval(1);

    /*
        Set this as userdata 
    */

    SetWindowLongPtr(win32_handle, GWLP_USERDATA, (intptr_t)this);
    glfwSetWindowUserPointer(glfw_handle, this);

    /*
        Set callbacks
    */

    glfwSetWindowSizeCallback(glfw_handle, window_context::resize_callback);
    glfwSetScrollCallback(glfw_handle, window_context::scroll_callback);
    glfwSetMouseButtonCallback(glfw_handle, window_context::mouse_button_callback);
    glfwSetCursorPosCallback(glfw_handle, window_context::mouse_move_callback);
    glfwSetWindowIconifyCallback(glfw_handle, window_context::iconify_callback);
    glfwSetDropCallback(glfw_handle, window_context::drop_callback);

    /*
        Center window on screen
    */

    glfwSetWindowMonitor(glfw_handle, NULL,
        (GetSystemMetrics(SM_CXSCREEN) / 2) - (settings.width  / 2),
        (GetSystemMetrics(SM_CYSCREEN) / 2) - (settings.height / 2),
         settings.width, settings.height, GLFW_DONT_CARE);

    // Prepare taskbar if possible
    prepare_taskbar();

    // Prepare system tray
    prepare_system_tray();

#ifndef NDEBUG
    IMGUI_CHECKVERSION();
#endif

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;

    if (m_settings.enable_docking)
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    if (m_settings.enable_multi_viewport)
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(LIBGUI_TO_NATIVE(m_native), true);
    LIBGUI_IMGUI_OPENGL_INIT();

    theme::init();
}

window::~window() {
    // Release taskbar if it exists
    release_taskbar();

    // Release system tray
    release_system_tray();

    LIBGUI_IMGUI_OPENGL_SHUTDOWN();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

window_title_bar& window::get_title_bar() {
    return m_title_bar;
}

const window_settings& window::get_settings() const {
    return m_settings;
}

void window::set_taskbar_status(taskbar_status status) {
    if (!m_native || !m_taskbar) return;

    auto win32_handle = glfwGetWin32Window(LIBGUI_TO_NATIVE(m_native));
    if (!win32_handle) return;

    ITaskbarList3* taskbar = (ITaskbarList3*)m_taskbar;
    taskbar->SetProgressState(win32_handle, (TBPFLAG)status);
}

void window::set_taskbar_progress(uint64_t progress) {
    if (!m_native || !m_taskbar) return;

    auto win32_handle = glfwGetWin32Window(LIBGUI_TO_NATIVE(m_native));
    if (!win32_handle) return;

    ITaskbarList3* taskbar = (ITaskbarList3*)m_taskbar;
    taskbar->SetProgressValue(win32_handle, (ULONGLONG)progress, 100);
}

void window::show() {
#ifndef NDEBUG
    DV_LOG_DEBG("", "OpenGL backend:  {}", libgui::debug::get_backend_type());
    DV_LOG_DEBG("", "OpenGL version:  {}", (char*)glGetString(GL_VERSION));
    DV_LOG_DEBG("", "OpenGL renderer: {}", (char*)glGetString(GL_RENDERER));
    DV_LOG_DEBG("", "ImGUI version:   {}", get_imgui_version());
#endif

    // Cancel showing if prepare failed
    if (!prepare()) return;

    loop();
}

void window::close() {
    glfwSetWindowShouldClose(LIBGUI_TO_NATIVE(m_native), 1);
}

void window::iconify() {
    glfwIconifyWindow(LIBGUI_TO_NATIVE(m_native));
}

void window::iconify_to_system_tray() {
    internal_iconify_to_system_tray();
}

void window::maximize_or_restore() {
    if (is_maximized()) {
        glfwRestoreWindow(LIBGUI_TO_NATIVE(m_native));
    }
    else {
        glfwMaximizeWindow(LIBGUI_TO_NATIVE(m_native));
    }
}

bool window::is_iconified() const {
    return is_iconified_to_taskbar() || is_iconified_to_system_tray();
}

bool window::is_iconified_to_taskbar() const {
    return glfwGetWindowAttrib(LIBGUI_TO_NATIVE(m_native), GLFW_ICONIFIED);
}

bool window::is_iconified_to_system_tray() const {
    return m_iconified_to_system_tray;
}

bool window::is_maximized() const {
    return glfwGetWindowAttrib(LIBGUI_TO_NATIVE(m_native), GLFW_MAXIMIZED);
}

///////////////////////////////////////////////////////////////////////////////
// PROTECTED

bool window::prepare() {
    return true;
}

void window::release() {}

void window::on_before_update() {}

void window::on_update() {}

void window::on_after_update() {}

void window::on_gui_before_update() {}

void window::on_gui_update() {}

void window::on_gui_after_update() {}

void window::on_resize(int width, int height) {}

void window::on_scroll(double dx, double dy) {}

void window::on_mouse_button(int btn, int action, int modifier) {}

void window::on_mouse_move(double dx, double dy) {}

void window::on_iconify(bool iconified) {}

void window::on_drop(int count, const char* paths[]) {}

void window::set_borderless() {
    auto win32_wnd = glfwGetWin32Window(LIBGUI_TO_NATIVE(m_native));
    if (!win32_wnd)
        return;

    window* dv_wnd = (window*)GetWindowLongPtr(win32_wnd, GWLP_USERDATA);
    if (!dv_wnd)
        return;

    // Save default callback
    dv_wnd->m_default_callback = (ptr_t)GetWindowLongPtr(win32_wnd, GWLP_WNDPROC);
    if (!dv_wnd->m_default_callback)
        return;

    // Get win32 window style
    auto wnd_style = GetWindowLongPtr(win32_wnd, GWL_STYLE);

    // Set window style
    wnd_style |= WS_OVERLAPPEDWINDOW;
    wnd_style |= WS_CAPTION;
    wnd_style |= WS_MAXIMIZEBOX;
    wnd_style |= WS_THICKFRAME;
    wnd_style |= CS_VREDRAW;
    wnd_style |= CS_HREDRAW;

    // Set win32 window style
    SetWindowLongPtr(win32_wnd, GWL_STYLE, wnd_style);

    // Set new callback
    SetWindowLongPtr(win32_wnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(window_context::wndproc_callback));

    // Force window redraw
    SetWindowPos(win32_wnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
}

void window::set_resizable(bool value) {
    m_resizable = value;

    if (m_resizable) {
        glfwSetWindowSizeLimits((GLFWwindow*)m_native, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE);
    }
    else {
        int width  = 0U;
        int height = 0U;

        glfwGetWindowSize((GLFWwindow*)m_native, &width, &height);
        glfwSetWindowSizeLimits((GLFWwindow*)m_native, width, height, width, height);
    }
}

void window::clear_frame_buffer(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

void window::loop() {
    while (!glfwWindowShouldClose(LIBGUI_TO_NATIVE(m_native))) {
    	if (is_iconified()) {
    		glfwWaitEvents();
    		continue;
    	}

        m_title_bar.skip_hit_testing = ImGui::IsAnyItemHovered();

    	// OpenGL render pass
    	on_before_update();
    	on_update();
    	on_after_update();

    	// GUI render pass
        LIBGUI_IMGUI_OPENGL_NEW_FRAME();
    	ImGui_ImplGlfw_NewFrame();
    	ImGui::NewFrame();

    	on_gui_before_update();
    	on_gui_update();

    	ImGui::Render();
        LIBGUI_IMGUI_OPENGL_RENDER(ImGui::GetDrawData());
        ImGui::EndFrame();

        if (m_settings.enable_multi_viewport) {
            GLFWwindow* backup_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_context);
        }

    	on_gui_after_update();

    	glfwSwapBuffers(LIBGUI_TO_NATIVE(m_native));
    	glfwPollEvents();
    }

    glfwDestroyWindow(LIBGUI_TO_NATIVE(m_native));
    release();
}

void window::prepare_taskbar() {
    if (!IsWindows7OrGreater()) return;

    HRESULT result = CoInitialize(NULL);
    if ((result != S_OK) && (result != S_FALSE))
        return;

    ITaskbarList3* taskbar = NULL;
    result = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&taskbar));

    if (!SUCCEEDED(result))
        return;

    result = taskbar->HrInit();

    if (!SUCCEEDED(result))
        return;

    m_taskbar = (ptr_t)taskbar;
}

void window::release_taskbar() {
    if (!m_taskbar) return;

    ITaskbarList3* taskbar = (ITaskbarList3*)m_taskbar;
    taskbar->Release();

    m_taskbar = nullptr;
}

void window::prepare_system_tray() {
    if (m_system_tray) return;

    auto win32_handle = glfwGetWin32Window(LIBGUI_TO_NATIVE(m_native));
    if (!win32_handle) return;

    NOTIFYICONDATA* system_tray = new NOTIFYICONDATA();
    system_tray->cbSize           = sizeof(NOTIFYICONDATA);
    system_tray->hWnd             = win32_handle;
    system_tray->uID              = 100;
    system_tray->uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    system_tray->uCallbackMessage = WM_USER_TRAYICON;
    system_tray->hIcon            = (HICON)GetClassLongPtr(win32_handle, GCLP_HICON);
    
    strncpy(system_tray->szTip, m_settings.title.c_str(), min(sizeof(system_tray->szTip), m_settings.title.size()));

    m_system_tray = (ptr_t)system_tray;
}

void window::release_system_tray() {
    if (!m_system_tray) return;

    internal_show_system_tray_icon();

    NOTIFYICONDATA* system_tray = (NOTIFYICONDATA*)m_system_tray;
    delete system_tray;
    m_system_tray = nullptr;
}

void window::internal_iconify_to_system_tray() {
    auto win32_handle = glfwGetWin32Window(LIBGUI_TO_NATIVE(m_native));
    if (!win32_handle) return;

    internal_show_system_tray_icon();
    ShowWindow(win32_handle, SW_HIDE);

    m_iconified_to_system_tray = true;
    window_context::iconify_callback(LIBGUI_TO_NATIVE(m_native), GLFW_TRUE);
}

void window::internal_restore_from_system_tray() {
    auto win32_handle = glfwGetWin32Window(LIBGUI_TO_NATIVE(m_native));
    if (!win32_handle) return;

    internal_hide_system_tray_icon();
    ShowWindow(win32_handle, SW_SHOW);
    SetForegroundWindow(win32_handle);

    m_iconified_to_system_tray = false;
    window_context::iconify_callback(LIBGUI_TO_NATIVE(m_native), GLFW_FALSE);
}

void window::internal_show_system_tray_icon() {
    if (!m_system_tray) return;

    Shell_NotifyIcon(NIM_ADD, (NOTIFYICONDATA*)m_system_tray);
}

void window::internal_hide_system_tray_icon() {
    if (!m_system_tray) return;

    Shell_NotifyIcon(NIM_DELETE, (NOTIFYICONDATA*)m_system_tray);
}

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

LRESULT window_context::wndproc_callback(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam) {
    window* wnd = (window*)GetWindowLongPtr(handle, GWLP_USERDATA);
    if (!wnd)
        throw std::runtime_error("");

    switch (msg) {
        case WM_NCCALCSIZE:
        {
            NCCALCSIZE_PARAMS* params = reinterpret_cast<NCCALCSIZE_PARAMS*>(lparam);

            WINDOWPLACEMENT wp{};
            wp.length = sizeof(WINDOWPLACEMENT);
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
        case WM_NCPAINT:
        {
            break;
        }
        case WM_NCHITTEST:
        {
            POINTS       mouse_pos        = MAKEPOINTS(lparam);
            POINT        client_mouse_pos = { mouse_pos.x, mouse_pos.y };
            ImGuiWindow* modal            = ImGui::GetTopMostAndVisiblePopupModal();

            ScreenToClient(handle, &client_mouse_pos);

            if (wnd->m_title_bar.mouseover_maximize_button)
                return HTMAXBUTTON;

            if (!modal && !wnd->m_title_bar.skip_hit_testing && wnd->m_title_bar.mouseover_title_bar)
                return HTCAPTION;

            LRESULT def_result = CallWindowProc((WNDPROC)wnd->m_default_callback, handle, msg, wparam, lparam);

            if (!wnd->m_resizable) {
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
        case WM_NCACTIVATE:
        {
            return TRUE;
        }
        case WM_GETMINMAXINFO:
        {
            MINMAXINFO* min_max = (MINMAXINFO*)lparam;

            // Get information about the monitor
            HMONITOR monitor = MonitorFromWindow(handle, MONITOR_DEFAULTTONEAREST);

            MONITORINFO monitor_info{};
            monitor_info.cbSize = sizeof(MONITORINFO);
            GetMonitorInfo(monitor, &monitor_info);

            // Set the maximum size based on monitor information
            min_max->ptMaxTrackSize.x = monitor_info.rcWork.right - monitor_info.rcWork.left;
            min_max->ptMaxTrackSize.y = monitor_info.rcWork.bottom - monitor_info.rcWork.top;
            min_max->ptMaxPosition.x  = 0;
            min_max->ptMaxPosition.y  = 0;

            break;
        }
        case WM_NCLBUTTONDOWN:
        {
            if (wparam == HTMAXBUTTON)
                return 0;

            if (wparam == HTCAPTION)
                ImGui::ClosePopupsExceptModals();

            break;
        }
        case WM_NCLBUTTONUP:
        {
            switch (wparam) {
                case HTMAXBUTTON:
                {
                    wnd->maximize_or_restore();
                    return 0;
                }

                default: break;
            }

            break;
        }
        case WM_USER_TRAYICON:
        {
            if (lparam == WM_LBUTTONDOWN) {
                wnd->internal_restore_from_system_tray();
            }
            break;
        }

        default: break;
    }

    return CallWindowProc((WNDPROC)wnd->m_default_callback, handle, msg, wparam, lparam);
}

void window_context::resize_callback(GLFWwindow* window, int width, int height) {
    if (width == 0 || height == 0) return;

    libgui::window* instance = static_cast<libgui::window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    glViewport(0, 0, width, height);
    instance->on_resize(width, height);
}

void window_context::scroll_callback(GLFWwindow* window, double dx, double dy) {
    libgui::window* instance = static_cast<libgui::window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    instance->on_scroll(dx, dy);
}

void window_context::mouse_button_callback(GLFWwindow* window, int button, int action, int modifier) {
    libgui::window* instance = static_cast<libgui::window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    instance->on_mouse_button(button, action, modifier);
}

void window_context::mouse_move_callback(GLFWwindow* window, double x, double y) {
    libgui::window* instance = static_cast<libgui::window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    instance->m_mouse_pos.dx = x - instance->m_mouse_pos.x;
    instance->m_mouse_pos.dy = instance->m_mouse_pos.y - y;
    instance->m_mouse_pos.x  = x;
    instance->m_mouse_pos.y  = y;

    instance->on_mouse_move(instance->m_mouse_pos.dx, instance->m_mouse_pos.dy);
}

void window_context::iconify_callback(GLFWwindow* window, int iconified) {
    libgui::window* instance = static_cast<libgui::window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    instance->on_iconify(iconified);
}

void window_context::drop_callback(GLFWwindow* window, int count, const char* paths[]) {
    libgui::window* instance = static_cast<libgui::window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    instance->on_drop(count, paths);
}
