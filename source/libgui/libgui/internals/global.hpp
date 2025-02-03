#pragma once

#include "libgui/defines/libgui.hpp"
#include "libgui/internals/backend/backend_life.hpp"

namespace libgui::internals {
    struct global {
        inline static int8_t       backend_type = LIBGUI_BACKEND_GLFW_OPENGL2;
        inline static backend_life backend      = {};
    };
}
