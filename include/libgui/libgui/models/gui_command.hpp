#pragma once

#include <functional>

namespace libgui {
    enum class gui_command_state : unsigned char {
        finished = 0,
        repeat   = 1
    };

    class gui_command {
    public:
        using command_fn_t = std::function<gui_command_state()>;

    public:
        bool to_execute = false;

    public:
        gui_command() = default;
        gui_command(command_fn_t fn);

    public:
        void execute();

    private:
        command_fn_t m_fn = nullptr;
    };
}