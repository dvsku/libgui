#include "libgui/defines/backends/opengl3.hpp"
#include "libgui/internals/contexts/backend_context_glfw_opengl3.hpp"

using namespace libgui::internals;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC
///////////////////////////////////////////////////////////////////////////////

libgui::result backend_context_glfw_opengl3::initialize(const window_settings& settings,
    const window_startup_settings& startup_settings) {
    libgui::result result;

    result = backend_context_glfw::initialize(settings, startup_settings);
    if (!result) {
        return result;
    }

    // Set event handlers
    internal_initialize_event_handlers();

    result.success = true;
    return result;
}

void backend_context_glfw_opengl3::teardown() {
    internal_teardown_event_handlers();
    backend_context_glfw::teardown();
}

void backend_context_glfw_opengl3::prepare_for_drawing() {

}

void backend_context_glfw_opengl3::draw() {
    glfwSwapBuffers(m_glfw_handle);
}

void backend_context_glfw_opengl3::clear(float r, float g, float b) {
    glClearColor(r, g, b, 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE
///////////////////////////////////////////////////////////////////////////////

void backend_context_glfw_opengl3::internal_initialize_event_handlers() {
    internal_event_attach<internals::ev::ev_frame_buffer_resized>([this](const auto& event) {
        internal_event_callback(event);
    });
}

void backend_context_glfw_opengl3::internal_teardown_event_handlers() {

}

///////////////////////////////////////////////////////////////////////////////
// BEHAVIOR EVENTS

void backend_context_glfw_opengl3::internal_event_callback(const internals::ev::ev_frame_buffer_resized& event) {
    glViewport(0, 0, event.width, event.height);
}
