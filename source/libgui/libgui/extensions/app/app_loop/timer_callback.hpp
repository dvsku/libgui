#pragma once

#include <chrono>
#include <functional>

namespace libgui::ex::app {
    struct timer_event {
        std::chrono::milliseconds delta  = std::chrono::milliseconds(0);
        mutable bool              cancel = false;
    };

    using timer_callback_t = std::function<void(const timer_event&)>;
}
