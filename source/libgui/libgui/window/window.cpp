#include "libgui/window/window.hpp"
#include "libgui/window/window_context.hpp"

using namespace libgui;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

window::window() : window_base() {}

window::~window() {
    teardown();
}

bool window::initialize(const window_settings& settings, const window_startup_settings& startup_settings) {
    // Create context
    m_context = std::make_unique<internals::window_context>(this, settings, startup_settings);
    if (!m_context) {
        return false;
        //LIBGUI_EXCEPTION_THROW("Failed to create context.");
    }

    // Initialize context
    if (!m_context->initialize(startup_settings)) {
        return false;
        //LIBGUI_EXCEPTION_THROW("Failed to initialize context.");
    }

    return true;

    //// Initialize ImGUI
    //if (!internal_initialize_imgui(startup_settings)) {
    //    LIBGUI_EXCEPTION_THROW("Failed to initialize ImGUI.");
    //}
}

void window::teardown() {
    if (m_context) {
        m_context.release();
    }
}
//
//void window::event_loop() {
//    // Cancel event loop if failed to initialize
//    if (!initialize())
//        return;
//
//    while (m_context && !m_context->is_closing()) {
//        /*
//            Process events
//        */
//        
//        if (is_minimized()) {
//            m_context->event_wait();
//            continue;
//        }
//
//        m_context->event_poll();
//
//        /*
//            Before ImGUI processing/rendering
//        */
//
//        on_before_update();
//        on_update();
//        on_after_update();
//
//        /*
//            ImGUI rendering
//        */
//
//        // Disable window dragging when:
//        //      Mouse is over an ImGUI item
//        //      A modal is visible
//        LIBGUI_SET_FLAG(m_titlebar_flags, LIBGUI_WTB_SKIP, ImGui::IsAnyItemHovered() || ImGui::GetTopMostAndVisiblePopupModal());
//
//        LIBGUI_IMGUI_OPENGL_NEW_FRAME();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        on_gui_before_update();
//        on_gui_update();
//
//        ImGui::Render();
//        LIBGUI_IMGUI_OPENGL_RENDER(ImGui::GetDrawData());
//        ImGui::EndFrame();
//
//        if (m_multi_viewport) {
//            auto backup_context = glfwGetCurrentContext();
//            ImGui::UpdatePlatformWindows();
//            ImGui::RenderPlatformWindowsDefault();
//            glfwMakeContextCurrent(backup_context);
//        }
//
//        on_gui_after_update();
//
//        /*
//            Swap buffers
//        */
//
//        glfwSwapBuffers(m_context->get_glfw_handle());
//    }
//
//    glfwDestroyWindow(m_context->get_glfw_handle());
//    teardown();
//}
//
//void window::event_loop_stop() {
//    if (!m_context)
//        return;
//    
//    m_context->request_close();
//}

void window::prepare_for_drawing() {

}

void window::draw() {
    glfwSwapBuffers(m_context->get_glfw_handle());
}


//void window::clear_frame_buffer(float r, float g, float b) {
//    glClearColor(r, g, b, 0xFF);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}
