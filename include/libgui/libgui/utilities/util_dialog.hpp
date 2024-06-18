#pragma once

#include <string>
#include <vector>

namespace libgui::dialog {
    std::string open_file(const std::string& title, const std::string& path,
        std::vector<std::string> filters = { "All Files", "*" });

    std::vector<std::string> open_file_multiple(const std::string& title, const std::string& path,
        std::vector<std::string> filters = { "All Files", "*" });

    std::string save_file(const std::string& title, const std::string& path,
        bool confirm_override = true, std::vector<std::string> filters = { "All Files", "*" });

    std::string select_dir(const std::string& title, const std::string& path);

    void message_failure(const std::string& title, const std::string& text);
}
