#include "libgui/internals/contexts/backend_context.hpp"
#include "libgui/window/window_base.hpp"

using namespace libgui;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

window_base::window_base()
    : internals::window_base() {}

window_base::~window_base() {}

window_settings window_base::get_settings() const {
    window_settings retval = {};

    if (!m_backend_context) {
        return retval;
    }

    auto settings = m_backend_context->get_settings();
    if (settings) {
        retval = *settings;
    }

    return retval;
}

void window_base::set_settings(const window_settings& settings) {
    if (!m_backend_context) {
        return;
    }

    m_backend_context->set_settings(settings);
}

void window_base::close(bool force) {
    if (!m_backend_context) {
        return;
    }

    m_backend_context->close(force);
}

void window_base::minimize_to_tb() {
    if (!m_backend_context) {
        return;
    }

    m_backend_context->set_minimize_to_tb(true);
}

void window_base::minimize_to_st() {
    if (!m_backend_context) {
        return;
    }

    m_backend_context->set_minimize_to_st(true);
}

void window_base::minimize_restore() {
    if (!m_backend_context) {
        return;
    }

    m_backend_context->set_minimize_to_tb(false);
    m_backend_context->set_minimize_to_st(false);
}

void window_base::maximize() {
    if (!m_backend_context) {
        return;
    }

    m_backend_context->set_maximized(true);
}

void window_base::maximize_restore() {
    if (!m_backend_context) {
        return;
    }

    m_backend_context->set_maximized(false);
}

void window_base::set_tb_state(taskbar_state state) {
    if (!m_backend_context) {
        return;
    }

    m_backend_context->set_tb_state(state);
}

void window_base::set_tb_value(uint64_t value) {
    if (!m_backend_context) {
        return;
    }

    m_backend_context->set_tb_value(value);
}

bool window_base::is_minimized() const {
    return is_minimized_to_tb() || is_minimized_to_st();
}

bool window_base::is_minimized_to_tb() const {
    if (!m_backend_context) {
        return false;
    }

    return m_backend_context->is_minimized_to_tb();
}

bool window_base::is_minimized_to_st() const {
    if (!m_backend_context) {
        return false;
    }

    return m_backend_context->is_minimized_to_st();
}

bool window_base::is_maximized() const {
    if (!m_backend_context) {
        return false;
    }

    return m_backend_context->is_maximized();
}

void window_base::event_poll() {
    if (!m_backend_context) {
        return;
    }

    m_backend_context->event_poll();
}

void window_base::clear(float r, float g, float b) {
    if (!m_backend_context) {
        return;
    }

    m_backend_context->clear(r, g, b);
}
