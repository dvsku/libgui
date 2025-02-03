#pragma once

#include "libgui/window/window_settings.hpp"

namespace libgui::ex::imgui {
    struct imgui_window_startup_settings : libgui::window_startup_settings {
        // Enable/disable docking.
        bool docking = true;

        // Enable/disable multi-viewport support.
        bool multi_viewport = false;
    };
}
