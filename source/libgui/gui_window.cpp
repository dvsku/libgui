#define GLFW_EXPOSE_NATIVE_WIN32
#define STRICT_TYPED_ITEMIDS

#include <windows.h>
#include <shlobj.h>
#include <stdexcept>
#include <versionhelpers.h>

#include "libgui/gui_window.hpp"
#include "libgui/utilities/util_debug.hpp"
#include "libutil/util_log.hpp"

#ifdef DV_USE_OPENGL2
    #include "backends/imgui_impl_opengl2.h"

    #define DV_IMGUI_OPENGL_INIT      ImGui_ImplOpenGL2_Init
    #define DV_IMGUI_OPENGL_SHUTDOWN  ImGui_ImplOpenGL2_Shutdown
    #define DV_IMGUI_OPENGL_NEW_FRAME ImGui_ImplOpenGL2_NewFrame
    #define DV_IMGUI_OPENGL_RENDER    ImGui_ImplOpenGL2_RenderDrawData
#elif  DV_USE_OPENGL3
    #include "glad/glad.h"
    #include "backends/imgui_impl_opengl3.h"

    #define DV_IMGUI_OPENGL_INIT      ImGui_ImplOpenGL3_Init
    #define DV_IMGUI_OPENGL_SHUTDOWN  ImGui_ImplOpenGL3_Shutdown
    #define DV_IMGUI_OPENGL_NEW_FRAME ImGui_ImplOpenGL3_NewFrame
    #define DV_IMGUI_OPENGL_RENDER    ImGui_ImplOpenGL3_RenderDrawData
#else
    #error Unsupported backend
#endif

#define DV_NATIVE         GLFWwindow*
#define DV_TO_NATIVE(ptr) (DV_NATIVE)ptr

#include "glfw/glfw3.h"
#include "glfw/glfw3native.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"

using namespace dvsku;

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

namespace dvsku {
    class dv_window_context {
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

static std::string get_imgui_version() {
    std::string str = std::string(ImGui::GetVersion());

#ifdef IMGUI_HAS_DOCK
    str += " docking";
#endif
#ifdef IMGUI_HAS_VIEWPORT
    str += " multi-viewport";
#endif

    return str;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dv_window::dv_window(const dv_window_settings& settings) {
    glfwSetErrorCallback(NULL);

    if (!glfwInit())
    	throw std::runtime_error("Failed to init glfw.");

    m_settings = settings;

    m_native = (ptr_t)glfwCreateWindow(settings.width, settings.height,
        settings.title.c_str(), NULL, NULL);

    if (!m_native)
    	throw std::runtime_error("Failed to create window.");

    DV_NATIVE native = DV_TO_NATIVE(m_native);

    // Get win32 handle
    auto win32_handle = glfwGetWin32Window(native);
    if (!win32_handle)
        throw std::runtime_error("Failed to create window.");

    // Set this as native handle userdata
    SetWindowLongPtr(win32_handle, GWLP_USERDATA, (intptr_t)this);

    glfwMakeContextCurrent(native);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(native, this);

    glfwSetWindowSizeCallback(native, dv_window_context::resize_callback);
    glfwSetScrollCallback(native, dv_window_context::scroll_callback);
    glfwSetMouseButtonCallback(native, dv_window_context::mouse_button_callback);
    glfwSetCursorPosCallback(native, dv_window_context::mouse_move_callback);
    glfwSetWindowIconifyCallback(native, dv_window_context::iconify_callback);
    glfwSetDropCallback(native, dv_window_context::drop_callback);

    glfwSetWindowMonitor(native, NULL,
        (GetSystemMetrics(SM_CXSCREEN) / 2) - (settings.width  / 2),
        (GetSystemMetrics(SM_CYSCREEN) / 2) - (settings.height / 2),
         settings.width, settings.height, GLFW_DONT_CARE);

#ifdef DV_USE_OPENGL3
    // Init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    	throw std::runtime_error("Failed to init glad.");
#endif

    // Prepare taskbar if possible
    prepare_taskbar();

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

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(DV_TO_NATIVE(m_native), true);
    DV_IMGUI_OPENGL_INIT();
}

dv_window::~dv_window() {
    // Release taskbar if it exists
    release_taskbar();

    glfwTerminate();

    DV_IMGUI_OPENGL_SHUTDOWN();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

dv_window_title_bar& dv_window::get_title_bar() {
    return m_title_bar;
}

const dv_window_settings& dv_window::get_settings() const {
    return m_settings;
}

void dv_window::set_taskbar_status(dv_taskbar_status status) {
    if (!m_native || !m_taskbar) return;

    auto win32_handle = glfwGetWin32Window(DV_TO_NATIVE(m_native));
    if (!win32_handle) return;

    ITaskbarList3* taskbar = (ITaskbarList3*)m_taskbar;
    taskbar->SetProgressState(win32_handle, (TBPFLAG)status);
}

void dv_window::set_taskbar_progress(uint64_t progress) {
    if (!m_native || !m_taskbar) return;

    auto win32_handle = glfwGetWin32Window(DV_TO_NATIVE(m_native));
    if (!win32_handle) return;

    ITaskbarList3* taskbar = (ITaskbarList3*)m_taskbar;
    taskbar->SetProgressValue(win32_handle, (ULONGLONG)progress, 100);
}

void dv_window::show() {
#ifndef NDEBUG
    DV_LOG_DEBG("", "OpenGL backend:  {}", dvsku::get_backend_type());
    DV_LOG_DEBG("", "OpenGL version:  {}", (char*)glGetString(GL_VERSION));
    DV_LOG_DEBG("", "OpenGL renderer: {}", (char*)glGetString(GL_RENDERER));
    DV_LOG_DEBG("", "ImGUI version:   {}", get_imgui_version());
#endif

    // Cancel showing if prepare failed
    if (!prepare()) return;

    loop();
}

void dv_window::close() {
    glfwSetWindowShouldClose(DV_TO_NATIVE(m_native), 1);
}

void dv_window::iconify() {
    glfwIconifyWindow(DV_TO_NATIVE(m_native));
}

void dv_window::maximize_or_restore() {
    if (is_maximized()) {
        glfwRestoreWindow(DV_TO_NATIVE(m_native));
    }
    else {
        glfwMaximizeWindow(DV_TO_NATIVE(m_native));
    }
}

bool dv_window::is_iconified() const {
    return glfwGetWindowAttrib(DV_TO_NATIVE(m_native), GLFW_ICONIFIED);
}

bool dv_window::is_maximized() const {
    return glfwGetWindowAttrib(DV_TO_NATIVE(m_native), GLFW_MAXIMIZED);
}

///////////////////////////////////////////////////////////////////////////////
// PROTECTED

bool dv_window::prepare() {
    return true;
}

void dv_window::release() {}

void dv_window::on_before_update() {}

void dv_window::on_update() {}

void dv_window::on_after_update() {}

void dv_window::on_gui_before_update() {}

void dv_window::on_gui_update() {}

void dv_window::on_gui_after_update() {}

void dv_window::on_resize(int width, int height) {}

void dv_window::on_scroll(double dx, double dy) {}

void dv_window::on_mouse_button(int btn, int action, int modifier) {}

void dv_window::on_mouse_move(double dx, double dy) {}

void dv_window::on_drop(int count, const char* paths[]) {}

void dv_window::set_borderless() {
    auto win32_wnd = glfwGetWin32Window(DV_TO_NATIVE(m_native));
    if (!win32_wnd)
        return;

    dv_window* dv_wnd = (dv_window*)GetWindowLongPtr(win32_wnd, GWLP_USERDATA);
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
    SetWindowLongPtr(win32_wnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(dv_window_context::wndproc_callback));

    // Force window redraw
    SetWindowPos(win32_wnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
}

void dv_window::set_resizable(bool value) {
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

void dv_window::clear_frame_buffer(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

void dv_window::loop() {
    while (!glfwWindowShouldClose(DV_TO_NATIVE(m_native))) {
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
    	DV_IMGUI_OPENGL_NEW_FRAME();
    	ImGui_ImplGlfw_NewFrame();
    	ImGui::NewFrame();

    	on_gui_before_update();
    	on_gui_update();

    	ImGui::Render();
    	DV_IMGUI_OPENGL_RENDER(ImGui::GetDrawData());
        ImGui::EndFrame();

        if (m_settings.enable_multi_viewport) {
            GLFWwindow* backup_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_context);
        }

    	on_gui_after_update();

    	glfwSwapBuffers(DV_TO_NATIVE(m_native));
    	glfwPollEvents();
    }

    glfwDestroyWindow(DV_TO_NATIVE(m_native));
    release();
}

void dv_window::prepare_taskbar() {
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

void dv_window::release_taskbar() {
    if (!m_taskbar) return;

    ITaskbarList3* taskbar = (ITaskbarList3*)m_taskbar;
    taskbar->Release();

    m_taskbar = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

LRESULT dv_window_context::wndproc_callback(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam) {
    dv_window* wnd = (dv_window*)GetWindowLongPtr(handle, GWLP_USERDATA);
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

        default: break;
    }

    return CallWindowProc((WNDPROC)wnd->m_default_callback, handle, msg, wparam, lparam);
}

void dv_window_context::resize_callback(GLFWwindow* window, int width, int height) {
    if (width == 0 || height == 0) return;

    dv_window* instance = static_cast<dv_window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    glViewport(0, 0, width, height);
    instance->on_resize(width, height);
}

void dv_window_context::scroll_callback(GLFWwindow* window, double dx, double dy) {
    dv_window* instance = static_cast<dv_window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    instance->on_scroll(dx, dy);
}

void dv_window_context::mouse_button_callback(GLFWwindow* window, int button, int action, int modifier) {
    dv_window* instance = static_cast<dv_window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    instance->on_mouse_button(button, action, modifier);
}

void dv_window_context::mouse_move_callback(GLFWwindow* window, double x, double y) {
    dv_window* instance = static_cast<dv_window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    instance->m_mouse_pos.dx = x - instance->m_mouse_pos.x;
    instance->m_mouse_pos.dy = instance->m_mouse_pos.y - y;
    instance->m_mouse_pos.x  = x;
    instance->m_mouse_pos.y  = y;

    instance->on_mouse_move(instance->m_mouse_pos.dx, instance->m_mouse_pos.dy);
}

void dv_window_context::iconify_callback(GLFWwindow* window, int iconified) {
    dv_window* instance = static_cast<dv_window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;
}

void dv_window_context::drop_callback(GLFWwindow* window, int count, const char* paths[]) {
    dv_window* instance = static_cast<dv_window*>(glfwGetWindowUserPointer(window));
    if (!instance) return;

    instance->on_drop(count, paths);
}
