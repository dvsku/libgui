#include "libgui/defines/os.hpp"
#include "libgui/exceptions/libgui_exception.hpp"
#include "libgui/internals/global.hpp"
#include "libgui/window/contexts/backend_context.hpp"

using namespace libgui::internals;

backend_context::backend_context(libgui::window_base* wnd, internals::window_base* wnd_internal)
    : m_wnd(wnd), m_wnd_internal(wnd_internal)
{
    libgui::result result = global::backend.initialize();
    if (!result) {
    #ifdef LIBGUI_OS_WINDOWS
        MessageBox(0, result.message.c_str(), "Failure", MB_OK | MB_ICONERROR);
    #endif

        LIBGUI_EXCEPTION_THROW(result.message);
    }
}

void backend_context::prepare_for_drawing() {

}

void backend_context::draw() {

}
