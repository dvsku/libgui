#pragma once

#include "libgui/event/event.hpp"

namespace libgui::ev {
    class window_event : public event {};
    class user_event   : public event {};

    // Sent on window close request.
    // This event is not sent when window::close() is called.
    // Source can be:
    //      Clicking on close (X) button
    //      ALT + F4
    //      Title bar context menu close
    //      etc..
    // 
    // Default behavior:
    //      Closes the window or minimizes to system tray if enabled in settings.
    // 
    // Cancel event in callback to disable/override default behavior.
    struct ev_close : window_event {};

    // Sent on window minimize request.
    // This event is not sent when window::minimize_to_*() is called.
    // Source can be:
    //      Clicking on minimize (-) button
    //      Title bar context menu minimize
    //      etc...
    // 
    // Default behavior:
    //      Minimizes the window to taskbar or to system tray if enabled in settings.
    // 
    // Cancel event in callback to disable/override default behavior.
    struct ev_minimize : window_event {};

    // Sent when window has been minimized.
    struct ev_minimized : window_event {};

    // Sent on window minimize restore request.
    // This event is not sent when window::minimize_restore() is called.
    // Source can be:
    //      Clicking icon in taskbar
    //      Clicking icon in system tray
    //      ALT + TAB
    //      etc...
    // 
    // Default behavior:
    //      Restores window from minimized state.
    // 
    // Cancel event in callback to disable/override default behavior.
    struct ev_minimize_restore : window_event {};

    // Sent when window has been restored from minimize.
    struct ev_minimize_restored : window_event {};

    // Sent on window maximize request.
    // This event is not sent when window::maximize() is called.
    // Source can be:
    //      Clicking on maximize button
    //      Title bar context menu maximize
    //      etc...
    // 
    //  Default behavior:
    //      Maximizes the window.
    // 
    // Cancel event in callback to disable/override default behavior.
    struct ev_maximize : window_event {};

    // Sent when window has been maximized.
    struct ev_maximized : window_event {};

    // Sent on window maximize restore request.
    // This event is not sent when window::maximize_restore() is called.
    // Source can be:
    //      Clicking on restore button
    //      Title bar context menu restore
    //      etc..
    //
    // Default behavior:
    //      Restores the window from maximized to windowed mode.
    //
    // Cancel event in callback to disable/override default behavior.
    struct ev_maximize_restore : window_event {};

    // Sent when window has been restored from maximized to windowed mode.
    struct ev_maximize_restored : window_event {};
}
