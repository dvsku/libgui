#include "libgui/models/gui_command.hpp"

using namespace libgui;

gui_command::gui_command(command_fn_t fn) {
    m_fn = fn;
}

void gui_command::execute() {
    if (!to_execute || !m_fn) return;

    if (m_fn() == gui_command_state::finished)
        to_execute = false;
}
