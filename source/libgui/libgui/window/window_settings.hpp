#pragma once

#include "libgui/misc/sizing.hpp"

#include <string>

namespace libgui {
    struct window_settings {
        // Window title.
        std::string title = "libgui window";

        // Minimize to system tray when
        // clicking on minimize (-) button in the titlebar.
        bool minimize_to_st_on_minimize = false;

        // Minimize to system tray when
        // clicking on close (x) button in the titlebar.
        bool minimize_to_st_on_close = false;
    };

    // Settings that are only applied on startup.
    struct window_startup_settings {
        // Window size.
        libgui::size_i size = { 1024, 768 };

        // Window minimum size.
        libgui::size_i minimum_size = { -1, -1 };

        // Is window resizable?
        // If TRUE the window can be resized.
        // If FALSE the window will be locked to initial width/height.
        //
        // If window is not resizable, it's also not maximizable.
        bool resizable = true;

        // Is the window centered on startup?
        bool centered = true;

        // Is the window maximized on startup?
        bool maximized = false;

        // Is the window minimized to system tray on startup?
        bool minimized_to_st = false;
    };
}
