#pragma once

#include "libgui/extensions/imgui/app/window/imgui_window_settings.hpp"
#include "libgui/extensions/imgui/defines/imgui.hpp"
#include "libgui/window/window_base.hpp"

namespace libgui::ex::imgui {
    class imgui_window : public libgui::window_base {
    public:
        imgui_window();
        virtual ~imgui_window();

    public:
        libgui::result initialize(const window_settings& settings = {},
            const imgui_window_startup_settings& startup_settings = {});

        void teardown();

    public:
        void prepare_for_drawing() override;
        void draw() override;
    };
}
