#include "libgui/defines/os.hpp"
#include "libgui/event/events_internal.hpp"
#include "libgui/exceptions/libgui_exception.hpp"
#include "libgui/internals/contexts/backend_context.hpp"
#include "libgui/internals/defines/window.hpp"
#include "libgui/internals/global.hpp"
#include "libgui/utilities/bit.hpp"

using namespace libgui;
using namespace libgui::internals;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

backend_context::backend_context(libgui::window_base* wnd, internals::window_base* wnd_internal)
    : m_wnd(wnd), m_wnd_internal(wnd_internal)
{
    libgui::result result = global::backend.initialize();
    if (!result) {
    #ifdef LIBGUI_OS_WINDOWS
        MessageBox(0, result.message.c_str(), "Failure", MB_OK | MB_ICONERROR);
    #endif
        
        LIBGUI_EXCEPTION_THROW("{}", result.message);
    }
}

window_settings* backend_context::get_settings() {
    return &m_settings;
}

void backend_context::close(bool force) {
    internals::ev::ev_close event;
    event.force = force;

    internal_event_enqueue(std::move(event));
}

void backend_context::set_minimize_to_tb(bool value) {
    if (value) {
        internal_event_enqueue<libgui::ev::ev_minimize_to_tb>({});
    }
    else {
        internal_event_enqueue<libgui::ev::ev_minimize_restore_from_tb>({});
    }
}

void backend_context::set_minimize_to_st(bool value) {
    if (value) {
        internal_event_enqueue<libgui::ev::ev_minimize_to_st>({});
    }
    else {
        internal_event_enqueue<libgui::ev::ev_minimize_restore_from_st>({});
    }
}

void backend_context::set_maximized(bool value) {
    if (value) {
        internal_event_enqueue<libgui::ev::ev_maximize>({});
    }
    else {
        internal_event_enqueue<libgui::ev::ev_maximize_restore>({});
    }
}

void backend_context::set_tb_state(taskbar_state state) {

}

void backend_context::set_tb_value(uint64_t value) {

}

bool backend_context::is_minimized_to_tb() const {
    return LIBGUI_HAS_FLAG(m_state, LIBGUI_WND_STATE_MINIMIZED_TO_TB);
}

bool backend_context::is_minimized_to_st() const {
    return LIBGUI_HAS_FLAG(m_state, LIBGUI_WND_STATE_MINIMIZED_TO_ST);
}

bool backend_context::is_maximized() const {
    return LIBGUI_HAS_FLAG(m_state, LIBGUI_WND_STATE_MAXIMIZED);
}

bool backend_context::is_resizable() const {
    return LIBGUI_HAS_FLAG(m_state, LIBGUI_WND_STATE_RESIZABLE);
}

void backend_context::prepare_for_drawing() {

}

void backend_context::draw() {

}

void backend_context::clear(float r, float g, float b) {

}
