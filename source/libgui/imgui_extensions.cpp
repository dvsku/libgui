#include "libgui/imgui_extensions.hpp"
#include "libgui/theme.hpp"

#include <libutil.hpp>

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

void libgui::imgui::text_ellipsis(const char* label, float max_w) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImGuiStyle&  style  = ImGui::GetStyle();

    const ImVec2 label_size = ImGui::CalcTextSize(label, 0, false);
    const ImVec2 pos        = window->DC.CursorPos;

    float max_x = window->WorkRect.Max.x;
    if (max_w != 0.0f)
        max_x = max_x = max_w > 0.0f ? pos.x + max_w : window->WorkRect.Max.x + max_w;

    const ImRect bb(pos, ImVec2(max_x, pos.y + label_size.y));
    ImGui::ItemSize(bb.GetSize());

    ImGui::RenderTextEllipsis(window->DrawList, pos,
        ImVec2(bb.Max.x, bb.Max.y + style.ItemSpacing.y), bb.Max.x, bb.Max.x, label, 0, &label_size);
}

bool libgui::imgui::link(const char* str, float max_w) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiStyle& style      = ImGui::GetStyle();
    const ImVec2      pos        = window->DC.CursorPos;
    const ImGuiID     id         = window->GetID(DV_FORMAT("{}##{}_{}", str, pos.x, pos.y).c_str());
    const ImVec2      label_size = ImGui::CalcTextSize(str, 0, false);

    float max_x = window->WorkRect.Max.x;
    if (max_w != 0.0f)
        max_x = max_w > 0.0f ? pos.x + max_w : window->WorkRect.Max.x + max_w;

    ImRect bb = ImRect(pos, ImVec2(max_x, pos.y + label_size.y));
    
    ImGui::ItemSize(bb.GetSize());
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

    const ImU32 col = 0;
    ImGui::RenderNavHighlight(bb, id);
    ImGui::RenderFrame(bb.Min, bb.Max, col, false, style.FrameRounding);

    if (held || pressed)
        theme::push_col(ImGuiCol_Text, theme::get_col(theme_col::link_activated));
    else
        theme::push_col(ImGuiCol_Text, theme::get_col(hovered ? theme_col::link_hovered : theme_col::link));

    ImGui::RenderTextEllipsis(window->DrawList, pos, ImVec2(bb.Max.x, bb.Max.y + style.ItemSpacing.y),
        bb.Max.x, bb.Max.x, str, 0, &label_size);

    theme::pop_col(1);

    if (hovered || pressed || held)
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

    return pressed;
}
