#include "libgui/window/window_context.hpp"
#include "libgui/window/window.hpp"
#include "libgui/exceptions/libgui_exception.hpp"
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
    // Create context
    m_context = std::make_unique<internals::window_context>(this, settings);
    if (!m_context) {
        LIBGUI_EXCEPTION_THROW("Failed to create context.");
    }

    // Initialize context
    if (!m_context->initialize()) {
        LIBGUI_EXCEPTION_THROW("Failed to initialize context.");
    }

    // Initialize ImGUI
    if (!internal_initialize_imgui()) {
        LIBGUI_EXCEPTION_THROW("Failed to initialize ImGUI.");
    }
}

window::~window() {
    internal_teardown_imgui();
}

window_settings window::get_settings() const {
    if (!m_context) return {};

    auto settings = m_context->get_settings();
    return settings ? *settings : window_settings();
}

void window::set_settings(const window_settings& settings) {
    if (!m_context) return;
    m_context->set_settings(settings);
}

void window::event_loop() {
    // Cancel event loop if failed to initialize
    if (!initialize())
        return;

    bool has_multi_viewport = false;

    {
        auto settings = m_context->get_settings();
        if (!settings) return;

        has_multi_viewport = settings->enable_multi_viewport;
    }

    while (m_context && !m_context->is_closing()) {
        /*
            Process events
        */
        
        if (is_minimized()) {
            m_context->event_wait();
            continue;
        }

        m_context->event_poll();

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

        if (has_multi_viewport) {
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

void window::event_loop_stop() {
    if (!m_context)
        return;
    
    m_context->request_close();
}

void window::close() {
    m_event_system.enqueue<libgui::ev::ev_close>({});
}

void window::minimize_to_tb() {
    if (!m_context)
        return;

    m_context->set_minimize_to_tb(true);
}

void window::minimize_to_st() {
    if (!m_context)
        return;

    m_context->set_minimize_to_st(true);
}

void window::minimize_restore() {
    if (!m_context)
        return;

    m_context->set_minimize_to_tb(false);
    m_context->set_minimize_to_st(false);
}

void window::maximize() {
    if (!m_context) return;
    m_context->set_maximized(true);
}

void window::maximize_restore() {
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
    auto settings = m_context->get_settings();
    if (!settings)
        return false;

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;

    if (settings->enable_docking)
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    if (settings->enable_multi_viewport)
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
