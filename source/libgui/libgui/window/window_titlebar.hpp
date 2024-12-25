#pragma once

namespace libgui::internals {
    class window_context;
}

namespace libgui {
    class window_titlebar {
    public:
        // Is the mouse over the title bar (not including minimize, maximize and close buttons)?
        // Used to determine draggable area.
        bool mouseover_titlebar = false;

        // Is the mouse over the minimize button?
        // Used to determine what the minimize button is.
        // Enables window show/hide animations.
        bool mouseover_minimize_button = false;

        // Is the mouse over the maximize/restore button?
        // Used to determine what the maximize/restore button is.
        // Enables snapping.
        bool mouseover_maximize_button = false;

        // Is the mouse over the close button?
        // Used to determine what the close button is.
        bool mouseover_close_button = false;

    private:
        friend class window;
        friend class internals::window_context;

    private:
        // Skip hit testing flag.
        bool m_skip_hit_testing = false;
    };
}
