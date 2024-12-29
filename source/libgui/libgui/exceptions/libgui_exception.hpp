#pragma once

#include <stdexcept>
#include <string>
#include <format>

#define LIBGUI_EXCEPTION_THROW(fmt, ...)    \
    throw libgui::libgui_exception(std::format(fmt, ##__VA_ARGS__))

namespace libgui {
    class libgui_exception : public std::runtime_error {
    public:
        libgui_exception()
            : std::runtime_error("") {}

        libgui_exception(const std::string& message)
            : std::runtime_error(message.c_str()) {}
    };
}
