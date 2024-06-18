#include "libgui/imgui_extensions.hpp"
#include "libgui/theme.hpp"

bool libgui::imgui::begin_composite(const char* id, const ImVec2& size, bool border, ImGuiWindowFlags flags) {
    ImVec2 size_arg = size;
    ImVec2 pos      = ImGui::GetCursorScreenPos();
    ImVec2 pos_max  = {};

    if (size.x == 0.0f)
        size_arg.x = ImGui::GetContentRegionAvail().x;

    if (size.y == 0.0f)
        size_arg.y = ImGui::GetContentRegionAvail().y;

    pos_max = { pos.x + size_arg.x, pos.y + size_arg.y };
    
    bool hovered = ImGui::IsMouseHoveringRect(pos, pos_max)
                   && !ImGui::IsAnyItemHovered()
                   && !ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId);

    theme::push_col(ImGuiCol_ChildBg, theme::get_col(hovered ? theme_col::composite_hovered : theme_col::composite));
    bool retval = ImGui::BeginChild(id, size, border, flags);
    theme::pop_col(1);

    return retval;
}

void libgui::imgui::end_composite() {
    ImGui::EndChild();
}

bool libgui::imgui::begin_item_context_menu(const char* id, ImGuiPopupFlags popup_flags) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 8.0f, 6.0f });
    theme::push_col(ImGuiCol_PopupBg, theme::get_col(theme_col::context_menu));

    ImGui::SetNextWindowSize({ 225.0f, 0.0f });

    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = g.CurrentWindow;

    if (window->SkipItems)
        return false;

    ImGuiID item_id = id ? window->GetID(id) : g.LastItemData.ID;
    IM_ASSERT(item_id != 0);

    int mouse_button = (popup_flags & ImGuiPopupFlags_MouseButtonMask_);
    if (ImGui::IsMouseReleased(mouse_button) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
        ImGui::OpenPopupEx(item_id, popup_flags);

    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_AlwaysAutoResize;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoSavedSettings;

    bool result = ImGui::BeginPopupEx(item_id, flags);

    ImGui::PopStyleVar(1);
    theme::pop_col();

    return result;
}

void libgui::imgui::end_item_context_menu() {
    ImGui::EndPopup();
}

bool libgui::imgui::begin_menu(const char* label, bool enabled) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 8.0f, 6.0f });
    ImGui::PushStyleColor(ImGuiCol_PopupBg, theme::get_col(theme_col::context_menu));

    ImGui::SetNextWindowSize({ 225.0f, 0.0f });
    bool result = ImGui::BeginMenu(label, enabled);

    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(1);

    return result;
}

void libgui::imgui::end_menu() {
    ImGui::EndMenu();
}

bool libgui::imgui::icon_button(const char* label, const ImVec2& size, const ImVec2& offset) {
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, offset);

    theme::push_col(ImGuiCol_Button,        theme::get_col(theme_col::icon_button));
    theme::push_col(ImGuiCol_ButtonHovered, theme::get_col(theme_col::icon_button_hovered));
    theme::push_col(ImGuiCol_ButtonActive,  theme::get_col(theme_col::icon_button_activated));
    theme::push_col(ImGuiCol_Text,          theme::get_col(theme_col::icon_button_text));

    bool retval = ImGui::Button(label, size);
    
    theme::pop_col(4);
    ImGui::PopStyleVar();

    return retval;
}

void libgui::imgui::text_ellipsis(const char* label, float max) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImGuiStyle&  style  = ImGui::GetStyle();

    const ImVec2 label_size = ImGui::CalcTextSize(label, 0, false);
    const ImVec2 pos        = window->DC.CursorPos;
    const ImVec2 padding    = style.SeparatorTextPadding;

    const float  separator_thickness = style.SeparatorTextBorderSize;
    const ImVec2 min_size(label_size.x + padding.x * 2.0f, ImMax(label_size.y + padding.y * 2.0f, separator_thickness));

    float max_x = window->WorkRect.Max.x;
    if (max != 0.0f)
        max_x = pos.x + max;

    const ImRect bb(pos, ImVec2(max_x, pos.y + min_size.y));

    const float sep1_x1 = pos.x;
    const float sep2_x2 = bb.Max.x;

    const float  label_avail_w = ImMax(0.0f, sep2_x2 - sep1_x1 - padding.x * 2.0f);
    const ImVec2 label_pos(pos.x, pos.y);

    if (label_size.x > 0.0f) {
        window->DC.CursorPosPrevLine.y = window->DC.CursorPos.y;

        ImGui::RenderTextEllipsis(window->DrawList, label_pos, ImVec2(bb.Max.x, bb.Max.y + style.ItemSpacing.y), bb.Max.x, bb.Max.x, label, 0, &label_size);
        window->DC.CursorPos.y += label_size.y + style.FramePadding.y * 2;
    }
}