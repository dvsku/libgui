#include "libgui/defines/backends/opengl3.hpp"
#include "libgui/defines/backends/glfw.hpp"
#include "libgui/internals/backend/backend_glfw_opengl3.hpp"
#include "libgui/internals/utilities/glfw.hpp"
#include "libgui/utilities/string.hpp"

using namespace libgui::internals;

libgui::result backend_glfw_opengl3::initialize() {
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

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    auto temp = glfwCreateWindow(1, 1, "", NULL, NULL);
    if (!temp) {
        result.message = LIBGUI_FORMAT("Failed to initialize GLAD.");
        return result;
    }

    glfwMakeContextCurrent(temp);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        result.message = LIBGUI_FORMAT("Failed to initialize GLAD.");
        return result;
    }

    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwDestroyWindow(temp);

    m_initialized = true;

    result.success = true;
    return result;
}

void backend_glfw_opengl3::teardown() {
    if (!m_initialized) {
        return;
    }

    glfwTerminate();

    m_initialized = false;
}
