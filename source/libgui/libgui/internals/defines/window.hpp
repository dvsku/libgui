#pragma once

// Is minimized to taskbar?
#define LIBGUI_WND_STATE_MINIMIZED_TO_TB    1 << 0

// Is minimized to system tray?
#define LIBGUI_WND_STATE_MINIMIZED_TO_ST    1 << 1

// Is system tray icon visible?
#define LIBGUI_WND_STATE_ST_ICON_VISIBLE    1 << 2

// Is window maximized?
#define LIBGUI_WND_STATE_MAXIMIZED          1 << 3

#define LIBGUI_WND_STATE_RESERVED_1         1 << 4
#define LIBGUI_WND_STATE_RESERVED_2         1 << 5

// Is window resizable?
#define LIBGUI_WND_STATE_RESIZABLE          1 << 6
