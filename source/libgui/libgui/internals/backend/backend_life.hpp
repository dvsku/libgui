#pragma once

#include "libgui/internals/backend/backend.hpp"
#include "libgui/misc/result.hpp"

#include <memory>

namespace libgui::internals {
    class backend_life final {
    public:
        backend_life()                    = default;
        backend_life(const backend_life&) = delete;
        backend_life(backend_life&&)      = delete;

        backend_life& operator=(const backend_life&) = delete;
        backend_life& operator=(backend_life&&)      = delete;

        ~backend_life();

    public:
        libgui::result initialize();

    private:
        std::unique_ptr<backend> m_backend = nullptr;

    private:
        void teardown();
    };
}
