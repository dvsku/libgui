#pragma once

#include <functional>

namespace libgui {
    class command {
    public:
        enum class state : unsigned char {
            finished = 0,
            repeat = 1
        };

        using command_fn_t = std::function<command::state()>;

    public:
        bool to_execute = false;

    public:
        command() = default;
        command(command_fn_t fn);

    public:
        void execute();

    private:
        command_fn_t m_fn = nullptr;
    };
}