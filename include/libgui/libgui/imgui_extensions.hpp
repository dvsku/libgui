#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace dvsku::imgui {
    bool begin_composite(const char* id, const ImVec2& size = { 0.0f, 0.0f }, bool border = false, ImGuiWindowFlags flags = 0);
    void end_composite();

    bool begin_item_context_menu(const char* id = nullptr, ImGuiPopupFlags popup_flags = 1);
    void end_item_context_menu();

    void text_ellipsis(const char* label, float max = 0.0f);
}
