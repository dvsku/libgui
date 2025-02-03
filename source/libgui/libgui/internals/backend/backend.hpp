#pragma once

#include "libgui/misc/result.hpp"

namespace libgui::internals {
    class backend {
    public:
        backend()               = default;
        backend(const backend&) = delete;
        backend(backend&&)      = delete;

        backend& operator=(const backend&) = delete;
        backend& operator=(backend&&)      = delete;

    public:
        virtual libgui::result initialize() = 0;
        virtual void           teardown()   = 0;

    protected:
        bool m_initialized = false;
    };
}
