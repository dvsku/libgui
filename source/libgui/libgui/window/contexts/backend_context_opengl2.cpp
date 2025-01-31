#include "libgui/defines/backends/opengl2.hpp"
#include "libgui/window/contexts/backend_context_opengl2.hpp"

using namespace libgui::internals;

void backend_context_opengl2::set_viewport(int32_t x, int32_t y, int32_t width, int32_t height) {
    glViewport(0, 0, width, height);
}

void backend_context_opengl2::clear_frame_buffer(float r, float g, float b) {
    glClearColor(r, g, b, 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
