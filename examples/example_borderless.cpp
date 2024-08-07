#include <libgui.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <iostream>

using namespace libgui;

class app : public libgui::window {
public:
    app(const libgui::window_settings& settings)
        : libgui::window(settings) {}

protected:
    bool prepare() override final {
        set_borderless();

        theme::add_font("default", 13.0f, {
            font_average_mono,
            font_fontawesome_solid,
            font_fontawesome_regular,
        });

        return true;
    }

    void on_gui_before_update()	override final {
        clear_frame_buffer(0.185f, 0.185f, 0.185f, 1.00f);
    }

    void on_gui_update() override final {
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoNav;
        flags |= ImGuiWindowFlags_NoDecoration;
        flags |= ImGuiWindowFlags_NoSavedSettings;
        flags |= ImGuiWindowFlags_NoMove;
        flags |= ImGuiWindowFlags_NoDocking;
        flags |= ImGuiWindowFlags_NoCollapse;
        flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        if (ImGui::Begin("Root##Component", NULL, flags)) {
            ImGui::PopStyleVar(1);

            ImVec2 title_bar_size    = { 0.0f, 25.0f };
            ImVec2 title_bar_padding = { 0.0f,  0.0f };

            ImGuiWindowFlags flags = 0;
            flags |= ImGuiWindowFlags_AlwaysUseWindowPadding;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, title_bar_padding);
            ImGui::PushStyleColor(ImGuiCol_ChildBg, 0xFF1F1F1F);

            ImRect rectangle = { {0.0f, 0.0f}, {0.0f, 0.0f} };

            if (ImGui::BeginChild("TitleBar##Component", title_bar_size, false, flags)) {
                auto max = ImGui::GetWindowContentRegionMax();
                auto min = ImGui::GetWindowContentRegionMin();

                ImGui::Indent(10.0f);

                ImGui::Dummy({ 0.0f, 2.0f });

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.77255f, 0.77255f, 0.77255f, 1.00f));
                ImGui::Text(get_settings().title.c_str());
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Button, 0x00FFFFFF);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0xFF3D3D3D);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0xFF3D3D3D);
                ImGui::PushStyleColor(ImGuiCol_Text, 0xFFC5C5C5);

                ImGui::SetCursorPosX(max.x - 90.0f);
                ImGui::SetCursorPosY(min.y - title_bar_padding.y);
                if (ImGui::Button(ICON_FA_MINUS"##MinimizeButton", { 30.0f, 25.0f })) {
                    iconify();
                }

                ImGui::SameLine(0.0f, 0.0f);
                ImGui::SetCursorPosY(min.y - title_bar_padding.y);
                if (!is_maximized()) {
                    ImGui::Button(ICON_FA_WINDOW_MAXIMIZE"##MaximizeButton", { 30.0f, 25.0f });
                }
                else {
                    ImGui::Button(ICON_FA_WINDOW_RESTORE"##RestoreButton", { 30.0f, 25.0f });
                }

                get_title_bar().mouseover_maximize_button = ImGui::IsItemHovered();

                ImGui::SameLine(0.0f, 0.0f);
                ImGui::SetCursorPosY(min.y - title_bar_padding.y);
                if (ImGui::Button(ICON_FA_XMARK"##ExitButton", { 30.0f, 25.0f })) {
                    close();
                }

                ImGui::PopStyleColor(4);

                ImGui::Unindent(10.0f);
                rectangle = ImGui::GetCurrentWindow()->Rect();
            }
            ImGui::EndChild();

            get_title_bar().mouseover_title_bar = ImGui::IsMouseHoveringRect(rectangle.Min, rectangle.Max);

            ImGui::PopStyleColor(1);
            ImGui::PopStyleVar(1);
        }
        ImGui::End();
    }
};

int main() {
    libutil::log::settings log_settings;
    log_settings.log_to_file = false;
    log_settings.level       = libutil::log::level::debug;

    libutil::log::init(log_settings);

    libgui::window_settings settings;
    settings.width  = 1024;
    settings.height = 768;
    settings.title  = std::string("Borderless ") + libgui::debug::get_backend_type();

    app app(settings);
    app.show();

    return 0;
}