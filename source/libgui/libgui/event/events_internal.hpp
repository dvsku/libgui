#pragma once

#include "libgui/event/event.hpp"

namespace libgui::internals::ev {
    class internal_event : public libgui::ev::event {};

    // Sent on all window restore requests.
    struct ev_restore : internal_event {};
}
