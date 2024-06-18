#pragma once

#include "libgui/fonts/font_source.hpp"
#include "libgui/fonts/embedded/average_mono.hpp"
#include "libgui/fonts/embedded/fontawesome_solid.hpp"

namespace {
    inline static libgui::font_config fontawesome_config = { 2, 1, { 0.0f, 0.0f }, { 0.0f, 0.0f }, 0, FLT_MAX, true };
}

namespace libgui {
    inline static font_source font_average_mono = font_source(__AVERAGE_MONO, __AVERAGE_MONO_SIZE);
    
    inline static font_source font_fontawesome_solid = font_source(__FONTAWESOME_SOLID, __FONTAWESOME_SOLID_SIZE, 
        { { ICON_MIN_FA, ICON_MAX_FA } }, fontawesome_config);
}
