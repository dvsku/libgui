#pragma once

#include "libgui/imgui.hpp"
#include "libgui/global.hpp"

namespace libgui::debug {
    constexpr auto get_imgui_version() {
    #if defined(IMGUI_HAS_DOCK) && defined(IMGUI_HAS_VIEWPORT)
        return IMGUI_VERSION " docking multi-viewport";
    #elif defined(IMGUI_HAS_DOCK)
        return IMGUI_VERSION " docking";
    #else
        return IMGUI_VERSION " multi-viewport";
    #endif
    }

    auto get_backend_type() {
        return global::backend.get_type();
    }

    auto get_backend_version() {
        return global::backend.get_version();
    }
}
