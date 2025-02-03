#include "libgui/extensions/imgui/app/internals/contexts/imgui_backend_context_glfw_opengl2.hpp"
#include "libgui/extensions/imgui/defines/imgui.hpp"
#include "libgui/internals/window/window_titlebar.hpp"
#include "libgui/utilities/string.hpp"

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl2.h>

using namespace libgui::ex::imgui::internals;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC
///////////////////////////////////////////////////////////////////////////////

imgui_backend_context_glfw_opengl2::imgui_backend_context_glfw_opengl2(libgui::window_base* wnd,
    libgui::internals::window_base* wnd_internal)
    : imgui_backend_context(), libgui::internals::backend_context_glfw_opengl2(wnd, wnd_internal) {}

libgui::result imgui_backend_context_glfw_opengl2::initialize(const window_settings& settings,
    const window_startup_settings& startup_settings)
{
    libgui::result result;

    result = libgui::internals::backend_context_glfw_opengl2::initialize(settings, startup_settings);
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

void imgui_backend_context_glfw_opengl2::teardown() {
    internal_teardown_imgui();
    imgui_backend_context::teardown();
    libgui::internals::backend_context_glfw_opengl2::teardown();
}

void imgui_backend_context_glfw_opengl2::prepare_for_drawing() {
    if (!m_wnd) {
        return;
    }

    // Disable titlebar dragging when:
    //      Mouse is over an ImGUI item
    //      A modal is visible
    LIBGUI_SET_FLAG(m_wnd->tb_flags, LIBGUI_WTB_SKIP, ImGui::IsAnyItemHovered() || ImGui::GetTopMostAndVisiblePopupModal());

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void imgui_backend_context_glfw_opengl2::draw() {
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
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

libgui::result imgui_backend_context_glfw_opengl2::internal_initialize_imgui(const imgui_window_startup_settings& startup_settings) {
    libgui::result result;

    if (!ImGui_ImplGlfw_InitForOpenGL(m_glfw_handle, true)) {
        result.message = "Failed to initialize ImGUI GLFW.";
        return result;
    }

    if (!ImGui_ImplOpenGL2_Init()) {
        result.message = "Failed to initialize ImGUI OpenGL.";
        return result;
    }

    result.success = true;
    return result;
}

void imgui_backend_context_glfw_opengl2::internal_teardown_imgui() {
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
}
