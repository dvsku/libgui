#include "libgui/extensions/imgui/app/internals/contexts/imgui_backend_context_glfw_opengl3.hpp"
#include "libgui/extensions/imgui/defines/imgui.hpp"
#include "libgui/internals/window/window_titlebar.hpp"
#include "libgui/utilities/string.hpp"

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

using namespace libgui::ex::imgui::internals;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC
///////////////////////////////////////////////////////////////////////////////

imgui_backend_context_glfw_opengl3::imgui_backend_context_glfw_opengl3(libgui::window_base* wnd,
    libgui::internals::window_base* wnd_internal)
    : libgui::internals::backend_context_glfw_opengl3(wnd, wnd_internal) {}

libgui::result imgui_backend_context_glfw_opengl3::initialize(const window_settings& settings,
    const window_startup_settings& startup_settings)
{
    libgui::result result;

    result = libgui::internals::backend_context_glfw_opengl3::initialize(settings, startup_settings);
    if (!result) {
        return result;
    }

    result = imgui_backend_context::initialize(settings,
        reinterpret_cast<const imgui_window_startup_settings&>(startup_settings));

    if (!result) {
        return result;
    }

    result = internal_initialize_imgui(reinterpret_cast<const imgui_window_startup_settings&>(startup_settings));
    if (!result) {
        return result;
    }

    result.success = true;
    return result;
}

void imgui_backend_context_glfw_opengl3::teardown() {
    internal_teardown_imgui();
    imgui_backend_context::teardown();
    libgui::internals::backend_context_glfw_opengl3::teardown();
}

void imgui_backend_context_glfw_opengl3::prepare_for_drawing() {
    if (!m_wnd) {
        return;
    }

    // Disable titlebar dragging when:
    //      Mouse is over an ImGUI item
    //      A modal is visible
    LIBGUI_SET_FLAG(m_wnd->tb_flags, LIBGUI_WTB_SKIP, ImGui::IsAnyItemHovered() || ImGui::GetTopMostAndVisiblePopupModal());

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void imgui_backend_context_glfw_opengl3::draw() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::EndFrame();

    if (m_multi_viewport) {
        auto backup_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_context);
    }

    glfwSwapBuffers(m_glfw_handle);
}

///////////////////////////////////////////////////////////////////////////////
// PROTECTED
///////////////////////////////////////////////////////////////////////////////

libgui::result imgui_backend_context_glfw_opengl3::internal_initialize_imgui(const imgui_window_startup_settings& startup_settings) {
    libgui::result result;

    if (!ImGui_ImplGlfw_InitForOpenGL(m_glfw_handle, true)) {
        result.message = "Failed to initialize ImGUI GLFW.";
        return result;
    }

    if (!ImGui_ImplOpenGL3_Init()) {
        result.message = "Failed to initialize ImGUI OpenGL.";
        return result;
    }

    result.success = true;
    return result;
}

void imgui_backend_context_glfw_opengl3::internal_teardown_imgui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
}
