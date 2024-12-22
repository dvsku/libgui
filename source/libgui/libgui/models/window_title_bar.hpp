#pragma once

namespace libgui {
    /*
        Custom title bar hit test config.
        Used by the underlying windowing system to determine parts of a custom title bar.
    */

    struct window_title_bar {
        /*
            Is the mouse over the custom title bar (not including iconify, maximize and close buttons)?
            Used to determine draggable area.
        */
        bool mouseover_title_bar = false;

        /*
            Is the mouse over the custom iconify button?
            Used to determine what the iconify button is. Provides show/hide animations.
        */
        bool mouseover_iconify_button = false;

        /*
            Is the mouse over the custom maximize/restore button?
            Used to determine what the maximize/restore button is. Provides snapping.
        */
        bool mouseover_maximize_button = false;

        /*
            Is the mouse over the custom close button?
            Used to determine what the close button is.
        */
        bool mouseover_close_button = false;

        /*
            Skip hit testing flag.
            If the flag is true, custom hit testing is skipped and default one is used.
        */
        bool skip_hit_testing = false;
    };
}
