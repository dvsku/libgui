#include "libgui/misc/work_context.hpp"
#include "libgui/window.hpp"
#include "libgui/utilities/util_sound.hpp"

using namespace libgui;

bool work_context::is_working() const {
    return m_working;
}

void work_context::start_working() {
    m_working     = true;
    cancel        = false;
    m_progress    = 0.0f;
    m_subprogress = 0.0f;

    m_window.set_taskbar_status(libgui::taskbar_status::normal);
    m_window.set_taskbar_progress(0U);
}

void work_context::stop_working(bool success_or_cancel) {
    m_working = false;
    cancel    = false;

    m_window.set_taskbar_status(libgui::taskbar_status::no_progress);
    m_window.set_taskbar_progress(0U);

    success_or_cancel ? libgui::sound::success() : libgui::sound::warning();
}

float work_context::get_progress() const {
    return m_progress;
}

float work_context::get_subprogress() const {
    return m_subprogress;
}

void work_context::update_progress(float change) {
    m_progress += change;
    m_window.set_taskbar_progress((uint64_t)m_progress);
}

void work_context::update_subprogress(float change) {
    m_subprogress += change;
}
