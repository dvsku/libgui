#pragma once

#include "libgui/window/window_base.hpp"

namespace libgui::internals {
    class backend_context {
    public:
        backend_context(libgui::window_base* wnd, internals::window_base* wnd_internal);

    public:
        // Initialize context.
        virtual result initialize(const window_settings& settings, const window_startup_settings& startup_settings) = 0;

        // Teardown context.
        virtual void teardown() = 0;

    public:
        // Prepare for incoming draw instructions.
        virtual void prepare_for_drawing();

        // Draw.
        virtual void draw();

    public:
        virtual void set_viewport(int32_t x, int32_t y, int32_t width, int32_t height) = 0;

        virtual void clear_frame_buffer(float r = 0.0f, float g = 0.0f, float b = 0.0f) = 0;

    protected:
        // Handles to libgui window instance.
        libgui::window_base*    m_wnd          = nullptr;
        internals::window_base* m_wnd_internal = nullptr;
    };
}
