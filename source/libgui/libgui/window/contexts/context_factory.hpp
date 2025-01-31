#pragma once

#include "libgui/window/contexts/backend_context.hpp"
#include "libgui/window/contexts/windowing_context.hpp"
#include "libgui/window/window_base.hpp"

namespace libgui::internals {
    class context_factory {
    public:
        using backend_ptr_t   = std::unique_ptr<backend_context>;
        using windowing_ptr_t = std::unique_ptr<windowing_context>;

    public:
        static backend_ptr_t   create_backend_context(libgui::window_base* wnd);
        static windowing_ptr_t create_windowing_context(libgui::window_base* wnd);
    };
}
