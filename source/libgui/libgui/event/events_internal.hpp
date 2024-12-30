#pragma once

#include "libgui/event/event.hpp"
#include "libgui/window/window_settings.hpp"

namespace libgui::internals::ev {
    class internal_event : public libgui::ev::event {};

    // Sent on all window restore requests.
    struct ev_restore : internal_event {};

    // Sent when frame buffer has been resized.
    struct ev_frame_buffer_resized : internal_event {
        int width  = 0;
        int height = 0;
    };

    // Sent when settings need updating.
    struct ev_update_settings : internal_event {
        window_settings settings = {};
    };
}
