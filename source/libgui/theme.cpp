#include "libgui/theme.hpp"

#include <imgui/imgui_internal.h>
#include <stdexcept>

using namespace libgui;

void theme::init() {
    ImGuiStyle& style = ImGui::GetStyle();

    style.TabRounding              = 0.0f;
    style.TabBarBorderSize         = 2.0f;
    style.DockingSeparatorSize     = 1.0f;
    style.WindowBorderSize         = 0.0f;
    style.WindowMenuButtonPosition = ImGuiDir_Right;

    set(ImGuiCol_Text,                  { 1.00f, 1.00f, 1.00f, 1.00f });
    set(ImGuiCol_TextDisabled,          { 0.50f, 0.50f, 0.50f, 1.00f });
    set(ImGuiCol_WindowBg,              { 0.14118f, 0.14118f, 0.14118f, 1.00f });
    set(ImGuiCol_ChildBg,               { 0.14118f, 0.14118f, 0.14118f, 1.00f });
    set(ImGuiCol_PopupBg,               { 0.14118f, 0.14118f, 0.14118f, 1.00f });
    set(ImGuiCol_Border,                { 0.43f, 0.43f, 0.50f, 0.50f });
    set(ImGuiCol_BorderShadow,          { 0.00f, 0.00f, 0.00f, 0.00f });
    set(ImGuiCol_FrameBg,               { 0.12157f, 0.12157f, 0.12157f, 1.00f });
    set(ImGuiCol_FrameBgHovered,        lighten(get(ImGuiCol_FrameBg)));
    set(ImGuiCol_FrameBgActive,         darken(get(ImGuiCol_FrameBg)));
    set(ImGuiCol_TitleBg,               { 0.12157f, 0.12157f, 0.12157f, 1.00f });
    set(ImGuiCol_TitleBgActive,         { 0.12157f, 0.12157f, 0.12157f, 1.00f });
    set(ImGuiCol_TitleBgCollapsed,      { 0.00f, 0.00f, 0.00f, 0.51f });
    set(ImGuiCol_MenuBarBg,             { 0.14118f, 0.14118f, 0.14118f, 1.00f });
    set(ImGuiCol_ScrollbarBg,           { 0.14118f, 0.14118f, 0.14118f, 1.00f });
    set(ImGuiCol_ScrollbarGrab,         { 0.29020f, 0.24314f, 0.61176f, 1.00f });
    set(ImGuiCol_ScrollbarGrabHovered,  lighten(get(ImGuiCol_ScrollbarGrab)));
    set(ImGuiCol_ScrollbarGrabActive,   darken(get(ImGuiCol_ScrollbarGrab)));
    set(ImGuiCol_CheckMark,             { 0.44314f, 0.37647f, 0.90980f, 1.00f });
    set(ImGuiCol_SliderGrab,            { 0.14118f, 0.14118f, 0.14118f, 1.00f });
    set(ImGuiCol_SliderGrabActive,      darken(get(ImGuiCol_SliderGrab)));
    set(ImGuiCol_Button,                { 0.29020f, 0.24314f, 0.61176f, 1.00f });
    set(ImGuiCol_ButtonHovered,         lighten(get(ImGuiCol_Button)));
    set(ImGuiCol_ButtonActive,          darken(get(ImGuiCol_Button)));
    set(ImGuiCol_Header,                { 0.29020f, 0.24314f, 0.61176f, 1.00f });
    set(ImGuiCol_HeaderHovered,         lighten(get(ImGuiCol_Header)));
    set(ImGuiCol_HeaderActive,          darken(get(ImGuiCol_Header)));
    set(ImGuiCol_Separator,             { 1.0f, 1.0f, 1.0f, 0.15f });
    set(ImGuiCol_SeparatorHovered,      lighten(get(ImGuiCol_Separator)));
    set(ImGuiCol_SeparatorActive,       darken(get(ImGuiCol_Separator)));
    set(ImGuiCol_ResizeGrip,            { 0.29020f, 0.24314f, 0.61176f, 1.00f });
    set(ImGuiCol_ResizeGripHovered,     lighten(get(ImGuiCol_ResizeGrip)));
    set(ImGuiCol_ResizeGripActive,      darken(get(ImGuiCol_ResizeGrip)));
    set(ImGuiCol_Tab,                   { 0.18039f, 0.18039f, 0.18039f, 1.00f });
    set(ImGuiCol_TabHovered,            lighten(get(ImGuiCol_Tab)));
    set(ImGuiCol_TabActive,             { 0.29020f, 0.24314f, 0.61176f, 1.00f });
    set(ImGuiCol_TabUnfocused,          { 0.18039f, 0.18039f, 0.18039f, 1.00f });
    set(ImGuiCol_TabUnfocusedActive,    { 0.29020f, 0.24314f, 0.61176f, 1.00f });
    set(ImGuiCol_DockingPreview,        RGBA_to_fRGBA(get(ImGuiCol_HeaderActive)) * ImVec4(1.0f, 1.0f, 1.0f, 0.7f));
    set(ImGuiCol_DockingEmptyBg,        { 0.20f, 0.20f, 0.20f, 1.00f });
    set(ImGuiCol_PlotLines,             { 0.61f, 0.61f, 0.61f, 1.00f });
    set(ImGuiCol_PlotLinesHovered,      lighten(get(ImGuiCol_PlotLines)));
    set(ImGuiCol_PlotHistogram,         { 0.29020f, 0.24314f, 0.61176f, 1.00f });
    set(ImGuiCol_PlotHistogramHovered,  get(ImGuiCol_PlotHistogram));
    set(ImGuiCol_TableHeaderBg,         { 0.19f, 0.19f, 0.20f, 1.00f });
    set(ImGuiCol_TableBorderStrong,     { 0.31f, 0.31f, 0.35f, 1.00f });
    set(ImGuiCol_TableBorderLight,      { 0.23f, 0.23f, 0.25f, 1.00f });
    set(ImGuiCol_TableRowBg,            { 0.00f, 0.00f, 0.00f, 0.00f });
    set(ImGuiCol_TableRowBgAlt,         { 1.00f, 1.00f, 1.00f, 0.06f });
    set(ImGuiCol_TextSelectedBg,        { 0.29020f, 0.24314f, 0.61176f, 0.35f });
    set(ImGuiCol_DragDropTarget,        { 1.00f, 1.00f, 0.00f, 0.90f });
    set(ImGuiCol_NavHighlight,          { 0.26f, 0.59f, 0.98f, 1.00f });
    set(ImGuiCol_NavWindowingHighlight, { 1.00f, 1.00f, 1.00f, 0.70f });
    set(ImGuiCol_NavWindowingDimBg,     { 0.80f, 0.80f, 0.80f, 0.20f });
    set(ImGuiCol_ModalWindowDimBg,      { 0.80f, 0.80f, 0.80f, 0.35f });
    
    /*
        ImGui extensions
    */

    set(theme_col::composite,         { 0.14118f, 0.14118f, 0.14118f, 1.00f });
    set(theme_col::composite_hovered, { 0.23922f, 0.23922f, 0.23922f, 1.00f });
    set(theme_col::context_menu,      { 0.21961f, 0.21961f, 0.21961f, 1.00f });
}

void theme::pop(size_t count) {
    count = std::min(count, m_backup_col.size());

    while (count) {
        backup_col& col = m_backup_col.back();

        if (col.type == std::type_index(typeid(ImGuiCol_)).hash_code()) {
            ImGui::PopStyleColor(1);
        }
        else {
            m_col[col.type][col.col] = col.value;
        }

        m_backup_col.pop_back();
        count--;
    }
}

////////////////////////////////////////////////////////////////////////////////
// FONTS

void theme::add_font(const std::string& id, float size, const std::vector<font_source>& fonts) {
    ImGuiIO&     io         = ImGui::GetIO();
    ImFontConfig config     = {};
    ImFont*      imgui_font = nullptr;

    config.SizePixels         = size;
    config.RasterizerMultiply = size < 15.0f ? 1.2f : 1.0f;

    for (auto& font : fonts) {
        m_font_sources.push_back(font);
    }

    for (size_t i = m_font_sources.size() - fonts.size(); i < m_font_sources.size(); i++) {
        const font_source& font = m_font_sources[i];
        
        if (font.filename == "" && (!font.data || !font.size))
            throw std::runtime_error("Font has no source.");

        config.MergeMode            = i != m_font_sources.size() - fonts.size() && fonts.size() > 1 ? true : false;
        config.FontDataOwnedByAtlas = font.filename != "" ? true : false;
        config.OversampleH          = font.config.oversample_h;
        config.OversampleV          = font.config.oversample_v;
        config.GlyphExtraSpacing    = font.config.extra_spacing;
        config.GlyphOffset          = font.config.offset;
        config.GlyphMinAdvanceX     = font.config.advance_x_match_size ? size : font.config.min_advance_x;
        config.GlyphMaxAdvanceX     = font.config.max_advance_x;

        ImFont* added_font = nullptr;
        if (font.filename != "") {
            added_font = io.Fonts->AddFontFromFileTTF(font.filename.c_str(), size, &config, (const ImWchar*)font.range.data());
        }
        else {
            added_font = io.Fonts->AddFontFromMemoryTTF(font.data, (int)font.size, size, &config, (const ImWchar*)font.range.data());
        }

        if (!config.MergeMode)
            imgui_font = added_font;
    }

    io.Fonts->Build();

    m_fonts[id] = imgui_font;
}

void theme::set_font(const std::string& id) {
    if (m_fonts.contains(id) && m_fonts[id]) {
        ImGui::SetCurrentFont(m_fonts[id]);
    }
}
