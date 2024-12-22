#include "libgui/backend.hpp"

#include <windows.h>

#ifdef LIBGUI_OPENGL2
    #include <gl/gl.h>
#elif  LIBGUI_OPENGL3
    #include <glad/glad.h>
#endif

#include <glfw/glfw3.h>
#include <stdexcept>

using namespace libgui;

backend::backend() {
    initialize();
}

backend::~backend() {
    teardown();
}

void backend::initialize() {
    if (m_is_initialized) return;

    glfwSetErrorCallback(NULL);

    if (!glfwInit()) {
        MessageBox(0, "Failed to initialize GLFW.", "Failure", MB_OK | MB_ICONERROR);
        throw std::runtime_error("Failed to initialize GLFW.");
    }
        
#ifdef LIBGUI_OPENGL3
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    auto temp = glfwCreateWindow(1, 1, "", NULL, NULL);
    if (!temp) {
        MessageBox(0, "Failed to initialize GLAD.", "Failure", MB_OK | MB_ICONERROR);
        throw std::runtime_error("Failed to initialize GLAD.");
    }

    glfwMakeContextCurrent(temp);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        MessageBox(0, "Failed to initialize GLAD.", "Failure", MB_OK | MB_ICONERROR);
        throw std::runtime_error("Failed to initialize GLAD.");
    }

    m_version = (char*)glGetString(GL_VERSION);

    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwDestroyWindow(temp);
#endif

    m_is_initialized = true;
}

void backend::teardown() {
    if (!m_is_initialized) return;

    glfwTerminate();

    m_is_initialized = false;
}

const char* backend::get_type() {
#ifdef LIBGUI_OPENGL2
    return "OpenGL2";
#else
    return "OpenGL3";
#endif
}

const char* backend::get_version() {
    return m_version.c_str();
}
