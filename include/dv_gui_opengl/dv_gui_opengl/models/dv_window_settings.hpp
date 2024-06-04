#pragma once

#include <string>
#include <cstdint>

namespace dvsku {
    struct dv_window_settings {
        uint32_t width  = 1024U;
        uint32_t height = 768U;

        std::string title = "dv_gui_window";

        bool enable_docking        = true;
        bool enable_multi_viewport = false;
    };
}