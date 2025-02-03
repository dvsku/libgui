#include "libgui/extensions/imgui/app/internals/contexts/imgui_backend_context.hpp"
#include "libgui/extensions/imgui/defines/imgui.hpp"

using namespace libgui::ex::imgui::internals;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC
///////////////////////////////////////////////////////////////////////////////

libgui::result imgui_backend_context::initialize(const window_settings& settings,
    const imgui_window_startup_settings& startup_settings)
{
    libgui::result result;

    // TODO: Change when supporting multiple windows
    if (ImGui::GetCurrentContext()) {
        result.success = true;
        return result;
    }

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;

    if (startup_settings.docking) {
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }

    if (startup_settings.multi_viewport) {
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    }

    m_multi_viewport = startup_settings.multi_viewport;

    //theme::init();

    result.success = true;
    return result;
}

void imgui_backend_context::teardown() {
    // TODO: Change when supporting multiple windows
    if (ImGui::GetCurrentContext()) {
        ImGui::DestroyContext();
    }
}
