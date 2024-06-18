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

    set_col(ImGuiCol_Text,                  { 1.00f, 1.00f, 1.00f, 1.00f });
    set_col(ImGuiCol_TextDisabled,          { 0.50f, 0.50f, 0.50f, 1.00f });
    set_col(ImGuiCol_WindowBg,              { 0.14118f, 0.14118f, 0.14118f, 1.00f });
    set_col(ImGuiCol_ChildBg,               { 0.14118f, 0.14118f, 0.14118f, 1.00f });
    set_col(ImGuiCol_PopupBg,               { 0.14118f, 0.14118f, 0.14118f, 1.00f });
    set_col(ImGuiCol_Border,                { 0.43f, 0.43f, 0.50f, 0.50f });
    set_col(ImGuiCol_BorderShadow,          { 0.00f, 0.00f, 0.00f, 0.00f });
    set_col(ImGuiCol_FrameBg,               { 0.12157f, 0.12157f, 0.12157f, 1.00f });
    set_col(ImGuiCol_FrameBgHovered,        lighten(get_col(ImGuiCol_FrameBg)));
    set_col(ImGuiCol_FrameBgActive,         darken(get_col(ImGuiCol_FrameBg)));
    set_col(ImGuiCol_TitleBg,               { 0.12157f, 0.12157f, 0.12157f, 1.00f });
    set_col(ImGuiCol_TitleBgActive,         { 0.12157f, 0.12157f, 0.12157f, 1.00f });
    set_col(ImGuiCol_TitleBgCollapsed,      { 0.00f, 0.00f, 0.00f, 0.51f });
    set_col(ImGuiCol_MenuBarBg,             { 0.14118f, 0.14118f, 0.14118f, 1.00f });
    set_col(ImGuiCol_ScrollbarBg,           { 0.14118f, 0.14118f, 0.14118f, 1.00f });
    set_col(ImGuiCol_ScrollbarGrab,         { 0.29020f, 0.24314f, 0.61176f, 1.00f });
    set_col(ImGuiCol_ScrollbarGrabHovered,  lighten(get_col(ImGuiCol_ScrollbarGrab)));
    set_col(ImGuiCol_ScrollbarGrabActive,   darken(get_col(ImGuiCol_ScrollbarGrab)));
    set_col(ImGuiCol_CheckMark,             { 0.44314f, 0.37647f, 0.90980f, 1.00f });
    set_col(ImGuiCol_SliderGrab,            { 0.29020f, 0.24314f, 0.61176f, 1.00f });
    set_col(ImGuiCol_SliderGrabActive,      darken(get_col(ImGuiCol_SliderGrab)));
    set_col(ImGuiCol_Button,                { 0.29020f, 0.24314f, 0.61176f, 1.00f });
    set_col(ImGuiCol_ButtonHovered,         lighten(get_col(ImGuiCol_Button)));
    set_col(ImGuiCol_ButtonActive,          darken(get_col(ImGuiCol_Button)));
    set_col(ImGuiCol_Header,                { 0.29020f, 0.24314f, 0.61176f, 1.00f });
    set_col(ImGuiCol_HeaderHovered,         lighten(get_col(ImGuiCol_Header)));
    set_col(ImGuiCol_HeaderActive,          darken(get_col(ImGuiCol_Header)));
    set_col(ImGuiCol_Separator,             { 1.0f, 1.0f, 1.0f, 0.15f });
    set_col(ImGuiCol_SeparatorHovered,      lighten(get_col(ImGuiCol_Separator)));
    set_col(ImGuiCol_SeparatorActive,       darken(get_col(ImGuiCol_Separator)));
    set_col(ImGuiCol_ResizeGrip,            { 0.29020f, 0.24314f, 0.61176f, 1.00f });
    set_col(ImGuiCol_ResizeGripHovered,     lighten(get_col(ImGuiCol_ResizeGrip)));
    set_col(ImGuiCol_ResizeGripActive,      darken(get_col(ImGuiCol_ResizeGrip)));
    set_col(ImGuiCol_Tab,                   { 0.18039f, 0.18039f, 0.18039f, 1.00f });
    set_col(ImGuiCol_TabHovered,            lighten(get_col(ImGuiCol_Tab)));
    set_col(ImGuiCol_TabActive,             { 0.29020f, 0.24314f, 0.61176f, 1.00f });
    set_col(ImGuiCol_TabUnfocused,          { 0.18039f, 0.18039f, 0.18039f, 1.00f });
    set_col(ImGuiCol_TabUnfocusedActive,    { 0.29020f, 0.24314f, 0.61176f, 1.00f });
    set_col(ImGuiCol_DockingPreview,        RGBA_to_fRGBA(get_col(ImGuiCol_HeaderActive)) * ImVec4(1.0f, 1.0f, 1.0f, 0.7f));
    set_col(ImGuiCol_DockingEmptyBg,        { 0.20f, 0.20f, 0.20f, 1.00f });
    set_col(ImGuiCol_PlotLines,             { 0.61f, 0.61f, 0.61f, 1.00f });
    set_col(ImGuiCol_PlotLinesHovered,      lighten(get_col(ImGuiCol_PlotLines)));
    set_col(ImGuiCol_PlotHistogram,         { 0.29020f, 0.24314f, 0.61176f, 1.00f });
    set_col(ImGuiCol_PlotHistogramHovered,  get_col(ImGuiCol_PlotHistogram));
    set_col(ImGuiCol_TableHeaderBg,         { 0.19f, 0.19f, 0.20f, 1.00f });
    set_col(ImGuiCol_TableBorderStrong,     { 0.31f, 0.31f, 0.35f, 1.00f });
    set_col(ImGuiCol_TableBorderLight,      { 0.23f, 0.23f, 0.25f, 1.00f });
    set_col(ImGuiCol_TableRowBg,            { 0.00f, 0.00f, 0.00f, 0.00f });
    set_col(ImGuiCol_TableRowBgAlt,         { 1.00f, 1.00f, 1.00f, 0.06f });
    set_col(ImGuiCol_TextSelectedBg,        { 0.29020f, 0.24314f, 0.61176f, 0.35f });
    set_col(ImGuiCol_DragDropTarget,        { 1.00f, 1.00f, 0.00f, 0.90f });
    set_col(ImGuiCol_NavHighlight,          { 0.26f, 0.59f, 0.98f, 1.00f });
    set_col(ImGuiCol_NavWindowingHighlight, { 1.00f, 1.00f, 1.00f, 0.70f });
    set_col(ImGuiCol_NavWindowingDimBg,     { 0.80f, 0.80f, 0.80f, 0.20f });
    set_col(ImGuiCol_ModalWindowDimBg,      { 0.80f, 0.80f, 0.80f, 0.35f });
    
    /*
        ImGui extensions
    */

    set_col(theme_col::composite,             { 0.14118f, 0.14118f, 0.14118f, 1.00f });
    set_col(theme_col::composite_hovered,     { 0.23922f, 0.23922f, 0.23922f, 1.00f });
    set_col(theme_col::context_menu,          { 0.18040f, 0.18040f, 0.18040f, 1.00f });
    set_col(theme_col::icon_button,           { 0.0f, 0.0f, 0.0f, 0.0f });
    set_col(theme_col::icon_button_hovered,   { 0.23922f, 0.23922f, 0.23922f, 1.00f });
    set_col(theme_col::icon_button_activated, get_col(theme_col::icon_button_hovered));
    set_col(theme_col::icon_button_text,      get_col(ImGuiCol_Text));
    set_col(theme_col::link,                  { 0.38824f, 0.32157f, 0.79608f, 1.00f });
    set_col(theme_col::link_hovered,          lighten(get_col(theme_col::link)));
    set_col(theme_col::link_activated,        darken(get_col(theme_col::link)));
}

void theme::pop_col(size_t count) {
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
