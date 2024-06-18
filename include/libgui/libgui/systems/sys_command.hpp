#pragma once

#include "libgui/models/command.hpp"
#include "libgui/type_traits.hpp"

#include <unordered_map>

namespace libgui {
    template<typename T>
    requires is_enum<T>
    class sys_command {
    public:
        sys_command()                   = default;
        sys_command(const sys_command&) = delete;
        sys_command(sys_command&&)      = delete;

        sys_command& operator=(const sys_command&) = delete;
        sys_command& operator=(sys_command&&)      = delete;

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
        void set_command(T command_type, const command& cmd) {
            if (m_commands.contains(command_type))
                return;

            m_commands.insert({ command_type, cmd });
        }

    private:
        std::unordered_map<T, command> m_commands;
    };
}