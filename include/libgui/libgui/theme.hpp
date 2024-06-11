#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui/imgui.h>
#include <unordered_map>

namespace dvsku {
    enum class theme_col {
        ////////////////////////////////////////////////////////////////////////
        // IMGUI NATIVE

        text,
        text_disabled,
        text_selected_background,
        window_background,
        child_background,
        popup_background,
        border,
        border_shadow,
        frame_background,
        title_background,
        title_background_collapsed,
        menu_bar_background,
        scroll,
        scroll_grab,
        check_mark,
        slide_grab,
        button,
        header,
        separator,
        resize,
        tab,
        tab_unfocused,
        docking_empty_background,
        plot_line,
        plot_histogram,
        table_header,
        table_border_strong,
        table_border_light,
        table_row,
        table_row_alt,
        nav_highlight,
        nav_highlight_window,
        nav_highlight_window_dim,
        modal_dim,
        drag_drop_target

        ////////////////////////////////////////////////////////////////////////
        // CUSTOM
    };

    constexpr auto RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return (((uint32_t)(a) << 24) | ((uint32_t)(b) << 16) | ((uint32_t)(g) << 8) | ((uint32_t)(r) << 0));
    }

    constexpr auto fRGBA_to_RGBA(float r, float g, float b, float a) {
        return RGBA(static_cast<uint8_t>(r * 255.0f), 
                    static_cast<uint8_t>(g * 255.0f), 
                    static_cast<uint8_t>(b * 255.0f), 
                    static_cast<uint8_t>(a * 255.0f));
    }

    constexpr auto RGBA_to_fRGBA(uint32_t rgba) {
        return ImVec4((rgba & 0xFF)         / 255.0f, 
                      ((rgba >> 8)  & 0xFF) / 255.0f, 
                      ((rgba >> 16) & 0xFF) / 255.0f, 
                      ((rgba >> 24) & 0xFF) / 255.0f);
    }

    constexpr auto lerp(uint32_t lhs, uint32_t rhs, float t) {
        auto a = RGBA_to_fRGBA(lhs);
        auto b = RGBA_to_fRGBA(rhs);

        return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
    }

    constexpr auto darken(uint32_t rgba) {
        return lerp(rgba, fRGBA_to_RGBA(0.0f, 0.0f, 0.0f, 1.00f), 0.2f);
    }

    constexpr auto lighten(uint32_t rgba) {
        return lerp(rgba, fRGBA_to_RGBA(1.0f, 1.0f, 1.0f, 1.00f), 0.1f);
    }

    class theme {
    public:
        static void init();

        static uint32_t set(theme_col col, uint32_t rgba);
        static uint32_t get(theme_col col);

        static void apply();

        static void save();
        static void restore();

    private:
        inline static std::unordered_map<theme_col, uint32_t> m_colors;
        inline static std::unordered_map<theme_col, uint32_t> m_saved_colors;
    };
}
