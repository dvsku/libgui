#pragma once

#include "libgui/utilities/bit.hpp"

#include <cstdint>

// Is the mouse over the title bar (not including minimize, maximize and close buttons)?
// Used to determine draggable area.
#define LIBGUI_WTB_MO_TB 1 << 0

// Is the mouse over the minimize button?
// Used to determine what the minimize button is.
#define LIBGUI_WTB_MO_MINIMIZE 1 << 1

// Is the mouse over the maximize/restore button?
// Used to determine what the maximize/restore button is.
// Enables snapping.
#define LIBGUI_WTB_MO_MAXIMIZE 1 << 2

// Is the mouse over the close button?
// Used to determine what the close button is.
#define LIBGUI_WTB_MO_CLOSE 1 << 3

namespace libgui {
    using window_titlebar_flags = int32_t;
}
