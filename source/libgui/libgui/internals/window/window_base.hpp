#pragma once

#include "libgui/event/event_system.hpp"
#include "libgui/event/events.hpp"

#include <memory>

namespace libgui::internals {
    class backend_context;
}

namespace libgui::internals {
    class window_base {
    public:
        std::unique_ptr<internals::backend_context> m_backend_context = nullptr;
        libgui::ev::event_system                    m_event_system    = {};
    };
}
