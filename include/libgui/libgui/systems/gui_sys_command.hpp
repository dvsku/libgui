#pragma once

#include "libgui/models/gui_command.hpp"
#include "libgui/type_traits.hpp"

#include <map>

namespace libgui {
    template<typename T>
    requires is_enum<T>
    class gui_sys_command {
    public:
        gui_sys_command()                      = default;
        gui_sys_command(const gui_sys_command&) = delete;
        gui_sys_command(gui_sys_command&&)      = delete;

        gui_sys_command& operator=(const gui_sys_command&) = delete;
        gui_sys_command& operator=(gui_sys_command&&)      = delete;

    public:
        // Set a command to execute
        void set_to_execute(T command_type) {
            if (!m_commands.contains(command_type))
                return;

            m_commands[command_type].to_execute = true;
        }

        // Check if command is set to execute
        bool& is_set_to_execute(T command_type) {
            return m_commands[command_type].to_execute;
        }

        // Execute all commands that are set to execute
        void execute_all() {
            for (auto& [type, command] : m_commands)
                command.execute();
        }

    protected:
        // Set command
        void set_command(T command_type, const gui_command& cmd) {
            if (m_commands.contains(command_type))
                return;

            m_commands.insert({ command_type, cmd });
        }

    private:
        std::map<T, gui_command> m_commands;
    };
}