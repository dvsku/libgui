#pragma once

#include "libgui/utilities/bit.hpp"

// Skip hit testing.
#define LIBGUI_WTB_SKIP 1 << 4

// Was mouse left click pressed down on minimize button?
#define LIBGUI_WTB_LDOWN_MINIMIZE 1 << 5

// Was mouse left click pressed down on maximize/restore button?
#define LIBGUI_WTB_LDOWN_MAXIMIZE 1 << 6

// Was mouse left click pressed down on close button?
#define LIBGUI_WTB_LDOWN_CLOSE 1 << 7

#define LIBGUI_WTB_LDOWN_ALL LIBGUI_WTB_LDOWN_MINIMIZE | LIBGUI_WTB_LDOWN_MAXIMIZE | LIBGUI_WTB_LDOWN_CLOSE
