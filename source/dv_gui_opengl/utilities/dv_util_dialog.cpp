#include "dv_gui_opengl/dv_gui_opengl/utilities/dv_util_dialog.hpp"
#include "pfd/portable-file-dialogs.h"

using namespace dvsku;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

std::string dv_util_dialog::open_file(const std::string& title, const std::string& path, 
    std::vector<std::string> filters)
{
    auto results = pfd::open_file(title, path, filters).result();
    return results.empty() ? "" : results[0];
}

std::vector<std::string> dv_util_dialog::open_file_multiple(const std::string& title, const std::string& path,
    std::vector<std::string> filters)
{
    return pfd::open_file(title, path, filters, pfd::opt::multiselect).result();
}

std::string dvsku::dv_util_dialog::save_file(const std::string& title, const std::string& path, 
    bool confirm_override, std::vector<std::string> filters) 
{
    return pfd::save_file(title, path, filters, confirm_override ? pfd::opt::none : pfd::opt::force_overwrite).result();
}

std::string dv_util_dialog::select_dir(const std::string& title, const std::string& path) {
    return pfd::select_folder(title, path).result();
}

void dv_util_dialog::message_error(const std::string& title, const std::string& text) {
    pfd::message(title, text, pfd::choice::ok, pfd::icon::error).result();
}