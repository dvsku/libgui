#include "libgui/extensions/imgui/app/internals/contexts/imgui_backend_context_glfw_opengl2.hpp"
#include "libgui/extensions/imgui/app/internals/contexts/imgui_backend_context_glfw_opengl3.hpp"
#include "libgui/extensions/imgui/app/internals/contexts/imgui_context_factory.hpp"
#include "libgui/internals/global.hpp"

using namespace libgui::ex::imgui::internals;

imgui_context_factory::backend_ptr_t imgui_context_factory::create_backend_context(libgui::window_base* wnd)
{
    libgui::internals::window_base* wnd_internal = (libgui::internals::window_base*)wnd;
    backend_ptr_t                   retval       = nullptr;

    switch (libgui::internals::global::backend_type) {
        case LIBGUI_BACKEND_GLFW_OPENGL2: {
            retval = std::make_unique<imgui_backend_context_glfw_opengl2>(wnd, wnd_internal);
            break;
        }
        case LIBGUI_BACKEND_GLFW_OPENGL3: {
            retval = std::make_unique<imgui_backend_context_glfw_opengl3>(wnd, wnd_internal);
            break;
        }

        default: break;
    }

    return retval;
}
