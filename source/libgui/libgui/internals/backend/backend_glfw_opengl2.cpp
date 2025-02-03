#include "libgui/defines/backends/opengl2.hpp"
#include "libgui/defines/backends/glfw.hpp"
#include "libgui/internals/backend/backend_glfw_opengl2.hpp"
#include "libgui/internals/utilities/glfw.hpp"
#include "libgui/utilities/string.hpp"

using namespace libgui::internals;

libgui::result backend_glfw_opengl2::initialize() {
    libgui::result result;

    if (m_initialized) {
        result.success = true;
        return result;
    }

    glfwSetErrorCallback(NULL);

    if (!glfwInit()) {
        result.message = LIBGUI_FORMAT("Failed to initialize GLFW. | {}", glfw::get_failure_desc());
        return result;
    }

    m_initialized = true;

    result.success = true;
    return result;
}

void backend_glfw_opengl2::teardown() {
    if (!m_initialized) {
        return;
    }

    glfwTerminate();

    m_initialized = false;
}
