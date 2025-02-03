#pragma once

#include "libgui/internals/contexts/backend_context_glfw.hpp"

namespace libgui::internals {
    class backend_context_glfw_opengl3 : public backend_context_glfw {
    public:
        backend_context_glfw_opengl3(libgui::window_base* wnd, internals::window_base* wnd_internal)
            : backend_context_glfw(wnd, wnd_internal) {}

    public:
        // Initialize context.
        virtual libgui::result initialize(const window_settings& settings,
            const window_startup_settings& startup_settings) override;

        // Teardown context.
        virtual void teardown() override;

    public:
        // Prepare for incoming draw instructions.
        virtual void prepare_for_drawing() override;

        // Draw.
        virtual void draw() override;

        // Clear window to specific color.
        void clear(float r, float g, float b) override;

    private:
        void internal_initialize_event_handlers();
        void internal_teardown_event_handlers();

    private:
        void internal_event_callback(const libgui::internals::ev::ev_frame_buffer_resized& event);
    };
}
