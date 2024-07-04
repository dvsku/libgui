#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace libgui::imgui {
    bool begin_composite(const char* id, const ImVec2& size = { 0.0f, 0.0f }, bool border = false,
        bool selected = false, ImGuiWindowFlags flags = 0);
    
    void end_composite();

    bool begin_item_context_menu(const char* id = nullptr, ImGuiPopupFlags popup_flags = 1);
    void end_item_context_menu();

    bool begin_menu(const char* label, bool enabled = true);
    void end_menu();

    bool icon_button(const char* label, const ImVec2& size = { 0.0f, 0.0f }, const ImVec2& offset = { 0.5f, 0.5f });

    void text_ellipsis(const char* label, float max_w = 0.0f);
    bool link(const char* str, float max_w = 0.0f);
}
