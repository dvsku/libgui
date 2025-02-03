#pragma once

#include "libgui/internals/contexts/backend_context.hpp"
#include "libgui/window/window_base.hpp"

namespace libgui::ex::imgui::internals {
    class imgui_context_factory {
    public:
        using backend_ptr_t = std::unique_ptr<libgui::internals::backend_context>;

    public:
        static backend_ptr_t create_backend_context(libgui::window_base* wnd);
    };
}
