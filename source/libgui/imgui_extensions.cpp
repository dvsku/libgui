#include "libgui/imgui_extensions.hpp"
#include "libgui/theme.hpp"

bool dvsku::imgui::begin_composite(const char* id, const ImVec2& size, bool border, ImGuiWindowFlags flags) {
    ImVec2 size_arg = {};
    ImVec2 pos      = ImGui::GetCursorScreenPos();
    ImVec2 pos_max  = {};

    if (size.x == 0.0f)
        size_arg.x = ImGui::GetContentRegionAvail().x;

    if (size.y == 0.0f)
        size_arg.y = ImGui::GetContentRegionAvail().y;

    pos_max = { pos.x + size_arg.x, pos.y + size_arg.y };
    
    if (!ImGui::IsMouseHoveringRect(pos, pos_max))
        ImGui::PushStyleColor(ImGuiCol_ChildBg, theme::get(theme_col::composite));
    else
        ImGui::PushStyleColor(ImGuiCol_ChildBg, lighten(theme::get(theme_col::composite)));

    bool retval = ImGui::BeginChild(id, size, border, flags);

    ImGui::PopStyleColor(1);

    return retval;
}

void dvsku::imgui::end_composite() {
    ImGui::EndChild();
}

void dvsku::imgui::text_ellipsis(const char* label, float max) {
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