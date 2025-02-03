#pragma once

#include "libgui/extensions/imgui/app/window/imgui_window_settings.hpp"
#include "libgui/misc/result.hpp"

namespace libgui::ex::imgui::internals {
    class imgui_backend_context {
    public:
        // Initialize context.
        libgui::result initialize(const window_settings& settings,
            const imgui_window_startup_settings& startup_settings);

        // Teardown context.
        void teardown();

    protected:
        bool m_multi_viewport = false;
    };
}
