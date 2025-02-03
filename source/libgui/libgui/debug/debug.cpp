#include "libgui/debug/debug.hpp"
#include "libgui/internals/global.hpp"

using namespace libgui::debug;

const char* libgui::debug::get_backend_type_string() {
    switch (internals::global::backend_type) {
        case LIBGUI_BACKEND_GLFW_OPENGL2: return "GLFW OpenGL2";
        case LIBGUI_BACKEND_GLFW_OPENGL3: return "GLFW OpenGL3";

        default: return "";
    }
}
