#pragma once

#include <string>

namespace libgui {
    constexpr auto get_backend_type() {
    #ifdef LIBGUI_OPENGL2
        return "OpenGL2";
    #else
        return "OpenGL3";
    #endif
    }
}
