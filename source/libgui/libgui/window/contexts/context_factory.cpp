#include "libgui/internals/global.hpp"
#include "libgui/window/contexts/backend_context_opengl2.hpp"
#include "libgui/window/contexts/backend_context_opengl3.hpp"
#include "libgui/window/contexts/context_factory.hpp"
#include "libgui/window/contexts/windowing_context_glfw.hpp"

using namespace libgui::internals;

context_factory::backend_ptr_t context_factory::create_backend_context(libgui::window_base* wnd)
{
    internals::window_base* wnd_internal = (internals::window_base*)wnd;
    backend_ptr_t           retval       = nullptr;

    switch (global::backend_type) {
        case LIBGUI_BACKEND_OPENGL2: {
            retval = std::make_unique<backend_context_opengl2>(wnd, wnd_internal);
            break;
        }
        case LIBGUI_BACKEND_OPENGL3: {
            retval = std::make_unique<backend_context_opengl3>(wnd, wnd_internal);
            break;
        }

        default: break;
    }

    return retval;
}

context_factory::windowing_ptr_t context_factory::create_windowing_context(libgui::window_base* wnd)
{
    internals::window_base* wnd_internal = (internals::window_base*)wnd;
    windowing_ptr_t         retval = nullptr;

    switch (global::windowing_type) {
        case LIBGUI_WINDOWING_GLFW: {
            retval = std::make_unique<windowing_context_glfw>(wnd, wnd_internal);
            break;
        }

        default: break;
    }

    return retval;
}
