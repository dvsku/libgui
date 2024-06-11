#include "libgui/theme.hpp"

using namespace dvsku;

void theme::init() {
    m_colors[theme_col::text]                       = fRGBA_to_RGBA(1.00f, 1.00f, 1.00f, 1.00f);
    m_colors[theme_col::text_disabled]              = fRGBA_to_RGBA(0.50f, 0.50f, 0.50f, 1.00f);
    m_colors[theme_col::text_selected_background]   = fRGBA_to_RGBA(0.29020f, 0.24314f, 0.61176f, 0.35f);
    m_colors[theme_col::window_background]          = fRGBA_to_RGBA(0.14118f, 0.14118f, 0.14118f, 1.00f);
    m_colors[theme_col::child_background]           = fRGBA_to_RGBA(0.14118f, 0.14118f, 0.14118f, 1.00f);
    m_colors[theme_col::popup_background]           = fRGBA_to_RGBA(0.14118f, 0.14118f, 0.14118f, 1.00f);
    m_colors[theme_col::border]                     = fRGBA_to_RGBA(0.43f, 0.43f, 0.50f, 0.50f);
    m_colors[theme_col::border_shadow]              = fRGBA_to_RGBA(0.00f, 0.00f, 0.00f, 0.00f);
    m_colors[theme_col::frame_background]           = fRGBA_to_RGBA(0.12157f, 0.12157f, 0.12157f, 1.00f);
    m_colors[theme_col::title_background]           = fRGBA_to_RGBA(0.12157f, 0.12157f, 0.12157f, 1.00f);
    m_colors[theme_col::title_background_collapsed] = fRGBA_to_RGBA(0.00f, 0.00f, 0.00f, 0.51f);
    m_colors[theme_col::menu_bar_background]        = fRGBA_to_RGBA(0.14118f, 0.14118f, 0.14118f, 1.00f);
    m_colors[theme_col::scroll]                     = fRGBA_to_RGBA(0.14118f, 0.14118f, 0.14118f, 1.00f);
    m_colors[theme_col::scroll_grab]                = fRGBA_to_RGBA(0.29020f, 0.24314f, 0.61176f, 1.00f);
    m_colors[theme_col::check_mark]                 = fRGBA_to_RGBA(0.44314f, 0.37647f, 0.90980f, 1.00f);
    m_colors[theme_col::slide_grab]                 = fRGBA_to_RGBA(0.29020f, 0.24314f, 0.61176f, 1.00f);
    m_colors[theme_col::button]                     = fRGBA_to_RGBA(0.29020f, 0.24314f, 0.61176f, 1.00f);
    m_colors[theme_col::header]                     = fRGBA_to_RGBA(0.0f, 0.0f, 0.0f, 0.0f);
    m_colors[theme_col::header_hovered]             = fRGBA_to_RGBA(0.29020f, 0.24314f, 0.61176f, 1.00f);
    m_colors[theme_col::separator]                  = fRGBA_to_RGBA(0.0f, 0.0f, 0.0f, 0.7f);
    m_colors[theme_col::resize]                     = fRGBA_to_RGBA(0.29020f, 0.24314f, 0.61176f, 1.00f);
    m_colors[theme_col::tab]                        = fRGBA_to_RGBA(0.29020f, 0.24314f, 0.61176f, 1.00f);
    m_colors[theme_col::tab_unfocused]              = fRGBA_to_RGBA(0.18039f, 0.18039f, 0.18039f, 1.00f);
    m_colors[theme_col::docking_empty_background]   = fRGBA_to_RGBA(0.20f, 0.20f, 0.20f, 1.00f);
    m_colors[theme_col::plot_line]                  = fRGBA_to_RGBA(0.61f, 0.61f, 0.61f, 1.00f);
    m_colors[theme_col::plot_histogram]             = fRGBA_to_RGBA(0.29020f, 0.24314f, 0.61176f, 1.00f);
    m_colors[theme_col::table_header]               = fRGBA_to_RGBA(0.19f, 0.19f, 0.20f, 1.00f);
    m_colors[theme_col::table_border_strong]        = fRGBA_to_RGBA(0.31f, 0.31f, 0.35f, 1.00f);
    m_colors[theme_col::table_border_light]         = fRGBA_to_RGBA(0.23f, 0.23f, 0.25f, 1.00f);
    m_colors[theme_col::table_row]                  = fRGBA_to_RGBA(0.00f, 0.00f, 0.00f, 0.00f);
    m_colors[theme_col::table_row_alt]              = fRGBA_to_RGBA(1.00f, 1.00f, 1.00f, 0.06f);
    m_colors[theme_col::nav_highlight]              = fRGBA_to_RGBA(0.26f, 0.59f, 0.98f, 1.00f);
    m_colors[theme_col::nav_highlight_window]       = fRGBA_to_RGBA(1.00f, 1.00f, 1.00f, 0.70f);
    m_colors[theme_col::nav_highlight_window_dim]   = fRGBA_to_RGBA(0.80f, 0.80f, 0.80f, 0.20f);
    m_colors[theme_col::modal_dim]                  = fRGBA_to_RGBA(0.80f, 0.80f, 0.80f, 0.35f);
    m_colors[theme_col::drag_drop_target]           = fRGBA_to_RGBA(1.00f, 1.00f, 0.00f, 0.90f);

    m_colors[theme_col::composite]                  = fRGBA_to_RGBA(0.14118f, 0.14118f, 0.14118f, 1.00f);
    m_colors[theme_col::composite_hovered]          = fRGBA_to_RGBA(0.23922f, 0.23922f, 0.23922f, 1.00f);
}

uint32_t theme::set(theme_col col, uint32_t rgba) {
    uint32_t prev = m_colors[col];
    m_colors[col] = rgba;
    return prev;
}

uint32_t theme::get(theme_col col) {
    return m_colors[col];
}

void theme::apply() {
    ImGuiStyle* style  = &ImGui::GetStyle();
    ImVec4*     colors = style->Colors;

    style->TabRounding              = 0.0f;
    style->TabBarBorderSize         = 2.0f;
    style->DockingSeparatorSize     = 1.0f;
    style->WindowBorderSize         = 0.0f;
    style->WindowMenuButtonPosition = ImGuiDir_Right;

    colors[ImGuiCol_Text]                  = RGBA_to_fRGBA(m_colors[theme_col::text]);
    colors[ImGuiCol_TextDisabled]          = RGBA_to_fRGBA(m_colors[theme_col::text_disabled]);
    colors[ImGuiCol_WindowBg]              = RGBA_to_fRGBA(m_colors[theme_col::window_background]);
    colors[ImGuiCol_ChildBg]               = RGBA_to_fRGBA(m_colors[theme_col::child_background]);
    colors[ImGuiCol_PopupBg]               = RGBA_to_fRGBA(m_colors[theme_col::popup_background]);
    colors[ImGuiCol_Border]                = RGBA_to_fRGBA(m_colors[theme_col::border]);
    colors[ImGuiCol_BorderShadow]          = RGBA_to_fRGBA(m_colors[theme_col::border_shadow]);
    colors[ImGuiCol_FrameBg]               = RGBA_to_fRGBA(m_colors[theme_col::frame_background]);
    colors[ImGuiCol_FrameBgHovered]        = lighten(m_colors[theme_col::frame_background]);
    colors[ImGuiCol_FrameBgActive]         = darken(m_colors[theme_col::frame_background]);
    colors[ImGuiCol_TitleBg]               = RGBA_to_fRGBA(m_colors[theme_col::title_background]);
    colors[ImGuiCol_TitleBgActive]         = RGBA_to_fRGBA(m_colors[theme_col::title_background]);
    colors[ImGuiCol_TitleBgCollapsed]      = RGBA_to_fRGBA(m_colors[theme_col::title_background_collapsed]);
    colors[ImGuiCol_MenuBarBg]             = RGBA_to_fRGBA(m_colors[theme_col::menu_bar_background]);
    colors[ImGuiCol_ScrollbarBg]           = RGBA_to_fRGBA(m_colors[theme_col::scroll]);
    colors[ImGuiCol_ScrollbarGrab]         = RGBA_to_fRGBA(m_colors[theme_col::scroll_grab]);
    colors[ImGuiCol_ScrollbarGrabHovered]  = lighten(m_colors[theme_col::scroll_grab]);
    colors[ImGuiCol_ScrollbarGrabActive]   = darken(m_colors[theme_col::scroll_grab]);
    colors[ImGuiCol_CheckMark]             = RGBA_to_fRGBA(m_colors[theme_col::check_mark]);
    colors[ImGuiCol_SliderGrab]            = RGBA_to_fRGBA(m_colors[theme_col::slide_grab]);
    colors[ImGuiCol_SliderGrabActive]      = darken(m_colors[theme_col::slide_grab]);
    colors[ImGuiCol_Button]                = RGBA_to_fRGBA(m_colors[theme_col::button]);
    colors[ImGuiCol_ButtonHovered]         = lighten(m_colors[theme_col::button]);
    colors[ImGuiCol_ButtonActive]          = darken(m_colors[theme_col::button]);
    colors[ImGuiCol_Header]                = RGBA_to_fRGBA(m_colors[theme_col::header]);
    colors[ImGuiCol_HeaderHovered]         = RGBA_to_fRGBA(m_colors[theme_col::header_hovered]);
    colors[ImGuiCol_HeaderActive]          = darken(m_colors[theme_col::header_hovered]);
    colors[ImGuiCol_Separator]             = RGBA_to_fRGBA(m_colors[theme_col::separator]);
    colors[ImGuiCol_SeparatorHovered]      = lighten(m_colors[theme_col::separator]);
    colors[ImGuiCol_SeparatorActive]       = darken(m_colors[theme_col::separator]);
    colors[ImGuiCol_ResizeGrip]            = RGBA_to_fRGBA(m_colors[theme_col::resize]);
    colors[ImGuiCol_ResizeGripHovered]     = lighten(m_colors[theme_col::resize]);
    colors[ImGuiCol_ResizeGripActive]      = darken(m_colors[theme_col::resize]);
    colors[ImGuiCol_Tab]                   = RGBA_to_fRGBA(m_colors[theme_col::tab_unfocused]);
    colors[ImGuiCol_TabHovered]            = lighten(m_colors[theme_col::tab]);
    colors[ImGuiCol_TabActive]             = RGBA_to_fRGBA(m_colors[theme_col::tab]);
    colors[ImGuiCol_TabUnfocused]          = RGBA_to_fRGBA(m_colors[theme_col::tab_unfocused]);
    colors[ImGuiCol_TabUnfocusedActive]    = RGBA_to_fRGBA(m_colors[theme_col::tab]);
    colors[ImGuiCol_DockingPreview]        = colors[ImGuiCol_HeaderActive] * ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
    colors[ImGuiCol_DockingEmptyBg]        = RGBA_to_fRGBA(m_colors[theme_col::docking_empty_background]);
    colors[ImGuiCol_PlotLines]             = RGBA_to_fRGBA(m_colors[theme_col::plot_line]);
    colors[ImGuiCol_PlotLinesHovered]      = lighten(m_colors[theme_col::plot_line]);
    colors[ImGuiCol_PlotHistogram]         = RGBA_to_fRGBA(m_colors[theme_col::plot_histogram]);
    colors[ImGuiCol_PlotHistogramHovered]  = colors[ImGuiCol_PlotHistogram];
    colors[ImGuiCol_TableHeaderBg]         = RGBA_to_fRGBA(m_colors[theme_col::table_header]);
    colors[ImGuiCol_TableBorderStrong]     = RGBA_to_fRGBA(m_colors[theme_col::table_border_strong]);
    colors[ImGuiCol_TableBorderLight]      = RGBA_to_fRGBA(m_colors[theme_col::table_border_light]);
    colors[ImGuiCol_TableRowBg]            = RGBA_to_fRGBA(m_colors[theme_col::table_row]);
    colors[ImGuiCol_TableRowBgAlt]         = RGBA_to_fRGBA(m_colors[theme_col::table_row_alt]);
    colors[ImGuiCol_TextSelectedBg]        = RGBA_to_fRGBA(m_colors[theme_col::text_selected_background]);
    colors[ImGuiCol_DragDropTarget]        = RGBA_to_fRGBA(m_colors[theme_col::drag_drop_target]);
    colors[ImGuiCol_NavHighlight]          = RGBA_to_fRGBA(m_colors[theme_col::nav_highlight]);
    colors[ImGuiCol_NavWindowingHighlight] = RGBA_to_fRGBA(m_colors[theme_col::nav_highlight_window]);
    colors[ImGuiCol_NavWindowingDimBg]     = RGBA_to_fRGBA(m_colors[theme_col::nav_highlight_window_dim]);
    colors[ImGuiCol_ModalWindowDimBg]      = RGBA_to_fRGBA(m_colors[theme_col::modal_dim]);
}

void theme::save() {
    for (auto& [col, value] : m_colors)
        m_saved_colors[col] = value;
}

void theme::restore() {
    for (auto& [col, value] : m_saved_colors)
        m_colors[col] = value;

    apply();
}
