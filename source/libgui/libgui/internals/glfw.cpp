#include "libgui/internals/glfw.hpp"
#include "libgui/defines/windowing.hpp"

using namespace libgui::internals;

std::string glfw::get_failure_desc() {
    const char* desc = nullptr;
    return glfwGetError(&desc) != GLFW_NO_ERROR && desc ? std::string(desc) : "";
}
