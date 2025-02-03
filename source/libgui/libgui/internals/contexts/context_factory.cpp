#include "libgui/internals/contexts/backend_context_glfw_opengl2.hpp"
#include "libgui/internals/contexts/backend_context_glfw_opengl3.hpp"
#include "libgui/internals/contexts/context_factory.hpp"
#include "libgui/internals/global.hpp"

using namespace libgui::internals;

context_factory::backend_ptr_t context_factory::create_backend_context(libgui::window_base* wnd)
{
    internals::window_base* wnd_internal = (internals::window_base*)wnd;
    backend_ptr_t           retval       = nullptr;

    switch (global::backend_type) {
        case LIBGUI_BACKEND_GLFW_OPENGL2: {
            retval = std::make_unique<backend_context_glfw_opengl2>(wnd, wnd_internal);
            break;
        }
        case LIBGUI_BACKEND_GLFW_OPENGL3: {
            retval = std::make_unique<backend_context_glfw_opengl3>(wnd, wnd_internal);
            break;
        }

        default: break;
    }

    return retval;
}
