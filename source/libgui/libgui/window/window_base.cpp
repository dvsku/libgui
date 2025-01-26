#include "libgui/window/window_context.hpp"
#include "libgui/window/window_base.hpp"

using namespace libgui;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

window_settings window_base::get_settings() const {
    if (!m_context) return {};

    auto settings = m_context->get_settings();
    return settings ? *settings : window_settings();
}

void window_base::set_settings(const window_settings& settings) {
    if (!m_context) return;
    m_context->set_settings(settings);
}

void window_base::close() {
    m_event_system.enqueue<libgui::ev::ev_close>({});
}

void window_base::minimize_to_tb() {
    if (!m_context)
        return;

    m_context->set_minimize_to_tb(true);
}

void window_base::minimize_to_st() {
    if (!m_context)
        return;

    m_context->set_minimize_to_st(true);
}

void window_base::minimize_restore() {
    if (!m_context)
        return;

    m_context->set_minimize_to_tb(false);
    m_context->set_minimize_to_st(false);
}

void window_base::maximize() {
    if (!m_context) return;
    m_context->set_maximized(true);
}

void window_base::maximize_restore() {
    if (!m_context) return;
    m_context->set_maximized(false);
}

void window_base::set_tb_state(taskbar_state state) {
    if (!m_context) return;
    m_context->set_tb_state(state);
}

void window_base::set_tb_value(uint64_t value) {
    if (!m_context) return;
    m_context->set_tb_value(value);
}

bool window_base::is_minimized() const {
    return is_minimized_to_tb() || is_minimized_to_st();
}

bool window_base::is_minimized_to_tb() const {
    if (!m_context) return false;
    return m_context->is_minimized_to_tb();
}

bool window_base::is_minimized_to_st() const {
    if (!m_context) return false;
    return m_context->is_minimized_to_st();
}

bool window_base::is_maximized() const {
    if (!m_context) return false;
    return m_context->is_maximized();
}

void window_base::event_poll() {
    if (!m_context) return;
    m_context->event_poll();
}

void window_base::clear(float r, float g, float b) {
    glClearColor(r, g, b, 0xFF);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
