#pragma once

#include "libgui/misc/sizing.hpp"

#include <windows.h>

namespace libgui::internals::win32 {
    // Get window default style.
    LONG_PTR get_window_default_style();

    // Get window DPI scaling.
    float get_window_dpi_scaling(HWND handle);

    // Get window DPI scaled non-client area size.
    libgui::rect_i get_window_nc_size(HWND handle, bool maximized = false);

    // Set window size.
    bool set_window_size(HWND handle, libgui::size_i size);

    // Set window position on the CURRENT monitor.
    //      pos    (0.0 - 1.0) : offset from monitor top left
    //      origin (0.0 - 1.0) : offset from window top left
    bool set_window_pos_relative(HWND handle, libgui::point_f pos, libgui::point_f origin = { 0.0f, 0.0f });
}
