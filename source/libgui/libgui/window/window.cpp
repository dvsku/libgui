#include "libgui/window/window_context.hpp"
#include "libgui/window/window.hpp"
#include "libgui/global.hpp"
#include "libgui/theme.hpp"
#include "libutil/log.hpp"

#ifdef LIBGUI_OPENGL2
    #include <imgui/backends/imgui_impl_opengl2.h>
    #include <imgui/backends/imgui_impl_glfw.h>

    #define LIBGUI_IMGUI_OPENGL_INIT      ImGui_ImplOpenGL2_Init
    #define LIBGUI_IMGUI_OPENGL_SHUTDOWN  ImGui_ImplOpenGL2_Shutdown
    #define LIBGUI_IMGUI_OPENGL_NEW_FRAME ImGui_ImplOpenGL2_NewFrame
    #define LIBGUI_IMGUI_OPENGL_RENDER    ImGui_ImplOpenGL2_RenderDrawData
#elif  LIBGUI_OPENGL3
    #include <imgui/backends/imgui_impl_opengl3.h>
    #include <imgui/backends/imgui_impl_glfw.h>

    #define LIBGUI_IMGUI_OPENGL_INIT      ImGui_ImplOpenGL3_Init
    #define LIBGUI_IMGUI_OPENGL_SHUTDOWN  ImGui_ImplOpenGL3_Shutdown
    #define LIBGUI_IMGUI_OPENGL_NEW_FRAME ImGui_ImplOpenGL3_NewFrame
    #define LIBGUI_IMGUI_OPENGL_RENDER    ImGui_ImplOpenGL3_RenderDrawData
#else
    #error Unsupported backend
#endif

#include <stdexcept>

using namespace libgui;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

window::window(const window_settings& settings) {
    global::backend.initialize();

    auto glfw_handle = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), NULL, NULL);
    if (!glfw_handle) {
        throw std::runtime_error("Failed to create window.");
    }

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
    m_context = std::make_unique<internals::window_context>(this, glfw_handle);
    if (!m_context) {
        throw std::runtime_error("Failed to create context.");
    }

    /*
        Create and initialize context
    */


    if (!m_context->initialize()) {
        throw std::runtime_error("Failed to initialize context.");
    }
    
    /*
        Save settings
    */

    set_settings(settings);

    /*
        Apply startup settings
    */

    if (m_settings.center_on_startup) {
        glfwSetWindowMonitor(glfw_handle, NULL,
            (GetSystemMetrics(SM_CXSCREEN) / 2) - (m_settings.width  / 2),
            (GetSystemMetrics(SM_CYSCREEN) / 2) - (m_settings.height / 2),
            m_settings.width, m_settings.height, GLFW_DONT_CARE);
    }

    if (m_settings.maximize_on_startup) {
        maximize();
    }

    if (m_settings.minimized_to_st_on_startup) {
        minimize_to_st();
    }

    /*
        Apply standard settings
    */

    internal_apply_settings();
    
    /*
        Initialize ImGUI
    */

    if (!internal_initialize_imgui()) {
        throw std::runtime_error("Failed to initialize ImGUI.");
    }

}

window::~window() {
    internal_teardown_imgui();
}

const window_settings& window::get_settings() const {
    return m_settings;
}

void window::set_settings(const window_settings& settings) {
    m_settings = settings;

    // TODO: Fire update event
}

void window::event_loop() {
    // Cancel event loop if failed to initialize
    if (!initialize())
        return;

    while (m_context && !m_context->is_closing()) {
        /*
            Process events
        */
        
        if (is_minimized()) {
            glfwWaitEvents();
            continue;
        }

        glfwPollEvents();

        /*
            Before ImGUI processing/rendering
        */

        on_before_update();
        on_update();
        on_after_update();

        /*
            ImGUI rendering
        */

        // Enable window dragging when mouse is not over an ImGUI item
        if (m_context->is_borderless()) {
            auto titlebar = get_titlebar();
            if (titlebar) {
                titlebar->m_skip_hit_testing = ImGui::IsAnyItemHovered();
            }
        }

        LIBGUI_IMGUI_OPENGL_NEW_FRAME();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        on_gui_before_update();
        on_gui_update();

        ImGui::Render();
        LIBGUI_IMGUI_OPENGL_RENDER(ImGui::GetDrawData());
        ImGui::EndFrame();

        if (m_settings.enable_multi_viewport) {
            auto backup_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_context);
        }

        on_gui_after_update();

        /*
            Swap buffers
        */

        glfwSwapBuffers(m_context->get_glfw_handle());
    }

    glfwDestroyWindow(m_context->get_glfw_handle());
    teardown();
}

void window::close() {
    if (!m_context) return;
    m_context->request_close();
}

void window::minimize_to_tb() {
    if (!m_context)
        return;

    m_context->minimize_to_tb();
}

void window::minimize_to_st() {
    if (!m_context)
        return;

    m_context->minimize_to_st();
}

void window::maximize() {
    if (!m_context) return;
    m_context->set_maximized(true);
}

void window::restore() {
    if (!m_context) return;
    m_context->set_maximized(false);
}

void window::set_tb_state(taskbar_state state) {
    if (!m_context) return;
    m_context->set_tb_state(state);
}

void window::set_tb_value(uint64_t value) {
    if (!m_context) return;
    m_context->set_tb_value(value);
}

bool window::is_minimized() const {
    return is_minimized_to_tb() || is_minimized_to_st();
}

bool window::is_minimized_to_tb() const {
    if (!m_context) return false;
    return m_context->is_minimized_to_tb();
}

bool window::is_minimized_to_st() const {
    if (!m_context) return false;
    return m_context->is_minimized_to_st();
}

bool window::is_maximized() const {
    if (!m_context) return false;
    return m_context->is_maximized();
}

///////////////////////////////////////////////////////////////////////////////
// PROTECTED

bool window::initialize() {
    return true;
}

void window::teardown() {}

window_titlebar* window::get_titlebar() {
    return nullptr;
}

void window::on_before_update() {}

void window::on_update() {}

void window::on_after_update() {}

void window::on_gui_before_update() {}

void window::on_gui_update() {}

void window::on_gui_after_update() {}

void window::on_resize(int width, int height) {}

void window::on_scroll(double dx, double dy) {}

void window::on_mouse_button(int button, int action, int modifier) {}

void window::on_mouse_move(double dx, double dy) {}

void window::on_minimize(bool minimized) {}

void window::on_drop(int count, const char* paths[]) {}

void window::clear_frame_buffer(float r, float g, float b) {
    glClearColor(r, g, b, 0xFF);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

bool window::internal_initialize_imgui() {
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;

    if (m_settings.enable_docking)
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    if (m_settings.enable_multi_viewport)
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    if (!ImGui_ImplGlfw_InitForOpenGL(m_context->get_glfw_handle(), true))
        return false;

    if (!LIBGUI_IMGUI_OPENGL_INIT())
        return false;

    theme::init();
    return true;
}

void window::internal_teardown_imgui() {
    LIBGUI_IMGUI_OPENGL_SHUTDOWN();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void window::internal_apply_settings() {
    if (!m_context)
        return;

    m_context->set_borderless(m_settings.borderless);
    m_context->set_resizable(m_settings.resizable);

    // Sync settings with current state

    m_settings.borderless = m_context->is_borderless();
    m_settings.resizable  = m_context->is_resizable();
}
