#pragma once

#include <string>

namespace dvsku {
    constexpr auto get_backend_type() {
    #ifdef DV_USE_OPENGL2
        return "OpenGL2";
    #else
        return "OpenGL3";
    #endif
    }
}
