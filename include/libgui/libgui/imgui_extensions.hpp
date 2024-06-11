#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace dvsku::imgui {
    bool begin_composite(const char* id, const ImVec2& size = { 0.0f, 0.0f }, bool border = false, ImGuiWindowFlags flags = 0);
    void end_composite();

    void text_ellipsis(const char* label, float max = 0.0f);
}
