#pragma once

#include "libgui/extensions/app/app_loop/timer_callback.hpp"

namespace libgui::ex::app::internals {
    struct timer_context {
        std::chrono::milliseconds interval = std::chrono::milliseconds(0);
        timer_callback_t          callback = nullptr;
    };
}
