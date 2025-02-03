#pragma once

#include "libgui/event/event.hpp"
#include "libgui/window/window_settings.hpp"

namespace libgui::internals::ev {
    class internal_event : public libgui::ev::event {};

    struct ev_close : internal_event {
        bool force = false;
    };




    // Sent when frame buffer has been resized.
    struct ev_frame_buffer_resized : internal_event {
        int width  = 0;
        int height = 0;
    };

    // Sent when settings need updating.
    struct ev_update_settings : internal_event {
        window_settings settings = {};
    };

    // Sent when startup settings need updating.
    struct ev_update_startup_settings : internal_event {
        window_startup_settings settings = {};
    };
}
