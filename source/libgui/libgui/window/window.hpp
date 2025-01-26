#pragma once

#include "libgui/window/window_base.hpp"

namespace libgui {
    class window : public window_base {
    public:
        window();
        virtual ~window();

    public:
        bool initialize(const window_settings& settings = {}, const window_startup_settings& startup_settings = {});
        void teardown();

    public:
        // Prepare window backend for incoming draw instructions.
        void prepare_for_drawing();

        // Execute draw instructions.
        void draw();
    };
}
