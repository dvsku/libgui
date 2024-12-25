#pragma once

#include <string>
#include <cstdint>

namespace libgui {
    struct window_settings {
        // Initial window width
        // Not affected by changes post startup
        uint32_t width = 1024U;

        // Initial window height
        // Not affected by changes post startup
        uint32_t height = 768U;

        // Window title
        std::string title = "libgui window";

        // Make window resizable
        // Non-resizable windows are locked to current width and height
        bool resizable = true;

        // Make window borderless
        // Borderless windows must implement custom title bar
        bool borderless = false;

        // Center window on startup
        // Not affected by changes post startup
        bool center_on_startup = true;

        // Maximize on startup
        // Not affected by changes post startup
        bool maximize_on_startup = false;

        // Minimized to system tray on startup
        // Not affected by changes post startup
        bool minimized_to_st_on_startup = false;

        // Enable/disable ImGUI docking
        bool enable_docking = true;

        // Enable/disable ImGUI multi-viewport support
        bool enable_multi_viewport = false;
    };
}
