#include "libgui/internals/utilities/glfw.hpp"
#include "libgui/defines/backends/glfw.hpp"

using namespace libgui::internals;

std::string glfw::get_failure_desc() {
    const char* desc = nullptr;
    return glfwGetError(&desc) != GLFW_NO_ERROR && desc ? std::string(desc) : "";
}
