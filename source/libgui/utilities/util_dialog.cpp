#include "libgui/utilities/util_dialog.hpp"
#include "pfd/portable-file-dialogs.h"

using namespace libgui;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

std::string util_dialog::open_file(const std::string& title, const std::string& path, 
    std::vector<std::string> filters)
{
    auto results = pfd::open_file(title, path, filters).result();
    return results.empty() ? "" : results[0];
}

std::vector<std::string> util_dialog::open_file_multiple(const std::string& title, const std::string& path,
    std::vector<std::string> filters)
{
    return pfd::open_file(title, path, filters, pfd::opt::multiselect).result();
}

std::string util_dialog::save_file(const std::string& title, const std::string& path, 
    bool confirm_override, std::vector<std::string> filters) 
{
    return pfd::save_file(title, path, filters, confirm_override ? pfd::opt::none : pfd::opt::force_overwrite).result();
}

std::string util_dialog::select_dir(const std::string& title, const std::string& path) {
    return pfd::select_folder(title, path).result();
}

void util_dialog::message_error(const std::string& title, const std::string& text) {
    pfd::message(title, text, pfd::choice::ok, pfd::icon::error).result();
}