#include "libgui/models/command.hpp"

using namespace libgui;

command::command(command_fn_t fn) {
    m_fn = fn;
}

void command::execute() {
    if (!to_execute || !m_fn) return;

    if (m_fn() == command::state::finished)
        to_execute = false;
}
