#pragma once

#include "libgui/event/event.hpp"

namespace libgui::internals::ev {
    class internal_event : public libgui::ev::event {};

    // Sent on all window restore requests.
    struct ev_restore : internal_event {};

    // Sent when frame buffer has been resized.
    struct ev_frame_buffer_resized : internal_event {
        int width  = 0;
        int height = 0;
    };
}
