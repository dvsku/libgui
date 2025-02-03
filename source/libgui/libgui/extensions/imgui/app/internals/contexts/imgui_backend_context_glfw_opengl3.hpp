#pragma once

#include "libgui/extensions/imgui/app/internals/contexts/imgui_backend_context.hpp"
#include "libgui/internals/contexts/backend_context_glfw_opengl3.hpp"
#include "libgui/extensions/imgui/app/window/imgui_window_settings.hpp"

namespace libgui::ex::imgui::internals {
    class imgui_backend_context_glfw_opengl3
        : public imgui_backend_context,
          public libgui::internals::backend_context_glfw_opengl3
    {
    public:
        imgui_backend_context_glfw_opengl3(libgui::window_base* wnd, libgui::internals::window_base* wnd_internal);

    public:
        // Initialize context.
        libgui::result initialize(const window_settings& settings,
            const window_startup_settings& startup_settings) override;

        // Teardown context.
        void teardown() override;

    public:
        // Prepare for incoming draw instructions.
        void prepare_for_drawing() override;

        // Draw.
        void draw() override;

    protected:
        libgui::result internal_initialize_imgui(const imgui_window_startup_settings& startup_settings);
        void           internal_teardown_imgui();
    };
}
