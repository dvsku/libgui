#pragma once

#include <imgui/imgui.h>
#include <typeindex>
#include <typeinfo>
#include <map>
#include <unordered_map>
#include <vector>

#include "libgui/fonts/font.hpp"

namespace libgui {
    ////////////////////////////////////////////////////////////////////////////
    // EXTENSION COLORS

    enum class theme_col {
        composite,
        composite_hovered,
        composite_selected,
        context_menu,
        icon_button,
        icon_button_hovered,
        icon_button_activated,
        icon_button_text,
        link,
        link_hovered,
        link_activated
    };

    ////////////////////////////////////////////////////////////////////////////
    // CONVERSIONS

    constexpr auto RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return (((uint32_t)(a) << 24) | ((uint32_t)(b) << 16) | ((uint32_t)(g) << 8) | ((uint32_t)(r) << 0));
    }

    constexpr auto fRGBA_to_RGBA(float r, float g, float b, float a) {
        return RGBA(static_cast<uint8_t>(r * 255.0f), 
                    static_cast<uint8_t>(g * 255.0f), 
                    static_cast<uint8_t>(b * 255.0f), 
                    static_cast<uint8_t>(a * 255.0f));
    }

    constexpr auto fRGBA_to_RGBA(ImVec4 value) {
        return fRGBA_to_RGBA(value.x, value.y, value.z, value.w);
    }

    constexpr auto RGBA_to_fRGBA(uint32_t rgba) {
        return ImVec4((rgba & 0xFF)         / 255.0f, 
                      ((rgba >> 8)  & 0xFF) / 255.0f, 
                      ((rgba >> 16) & 0xFF) / 255.0f, 
                      ((rgba >> 24) & 0xFF) / 255.0f);
    }

    ////////////////////////////////////////////////////////////////////////////
    // MODIFICATIONS

    constexpr auto lerp(uint32_t lhs, uint32_t rhs, float t) {
        auto a = RGBA_to_fRGBA(lhs);
        auto b = RGBA_to_fRGBA(rhs);

        return fRGBA_to_RGBA(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
    }

    constexpr auto darken(uint32_t rgba, float factor = 0.2f) {
        return lerp(rgba, fRGBA_to_RGBA(0.0f, 0.0f, 0.0f, 1.00f), factor);
    }

    constexpr auto lighten(uint32_t rgba, float factor = 0.1f) {
        return lerp(rgba, fRGBA_to_RGBA(1.0f, 1.0f, 1.0f, 1.00f), factor);
    }

    ////////////////////////////////////////////////////////////////////////////
    // THEME

    class theme {
    public:
        struct backup_col {
            size_t   type  = 0U;
            int      col   = 0;
            uint32_t value = {};
        };

    public:
        static void init();

        template<typename Col>
        static void push_col(Col col, uint32_t value) {
            m_backup_col.push_back({});
            m_backup_col.back().type = std::type_index(typeid(Col)).hash_code();
            m_backup_col.back().col  = (int)col;

            if (typeid(Col) == typeid(ImGuiCol_)) {
                ImGui::PushStyleColor((int)col, value);
            }
            else {
                m_backup_col.back().value = m_col[m_backup_col.back().type][m_backup_col.back().col];
                m_col[m_backup_col.back().type][m_backup_col.back().col] = value;
            }
        }

        static void pop_col(size_t count = 1);

        template<typename Col>
        static void set_col(Col col, uint32_t value) {
            if (typeid(Col) == typeid(ImGuiCol_)) {
                ImGui::GetStyle().Colors[(int)col] = RGBA_to_fRGBA(value);
            }
            else {
                m_col[std::type_index(typeid(Col)).hash_code()][(int)col] = value;
            }
        }

        template<typename Col>
        static void set_col(Col col, ImVec4 value) {
            if (typeid(Col) == typeid(ImGuiCol_)) {
                ImGui::GetStyle().Colors[(int)col] = value;
            }
            else {
                m_col[std::type_index(typeid(Col)).hash_code()][(int)col] = fRGBA_to_RGBA(value);
            }
        }

        template<typename Col>
        static uint32_t get_col(Col col) {
            if (typeid(Col) == typeid(ImGuiCol_)) {
                return fRGBA_to_RGBA(ImGui::GetStyle().Colors[(int)col]);
            }
            else {
                return m_col[std::type_index(typeid(Col)).hash_code()][(int)col];
            }
        }

        ////////////////////////////////////////////////////////////////////////
        // FONTS

        static void add_font(const std::string& id, float size, const std::vector<font_source>& fonts);
        static void set_font(const std::string& id);

    private:
        inline static std::unordered_map<size_t, std::unordered_map<int, uint32_t>> m_col          = {};
        inline static std::vector<backup_col>                                       m_backup_col   = {};
        inline static std::vector<font_source>                                      m_font_sources = {};
        inline static std::map<std::string, ImFont*>                                m_fonts        = {};
    };
}
