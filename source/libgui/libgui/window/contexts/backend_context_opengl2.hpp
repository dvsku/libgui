#pragma once

#include "libgui/window/contexts/backend_context.hpp"

namespace libgui::internals {
    class backend_context_opengl2 : public backend_context {
    public:
        backend_context_opengl2(libgui::window_base* wnd, internals::window_base* wnd_internal)
            : backend_context(wnd, wnd_internal) {}

    public:
        void set_viewport(int32_t x, int32_t y, int32_t width, int32_t height) override final;

        void clear_frame_buffer(float r = 0.0f, float g = 0.0f, float b = 0.0f) override final;
    };
}
