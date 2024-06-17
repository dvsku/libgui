#include <libgui.hpp>
#include <iostream>

class app : public libgui::gui_window {
public:
    app(const libgui::gui_window_settings& settings)
        : libgui::gui_window(settings) {}
};

int main() {
    dvsku::util_log::settings log_settings;
    log_settings.log_to_file = false;
    log_settings.level       = dvsku::util_log::level::debug;

    dvsku::util_log::init(log_settings);
    dvsku::util_log::create_source("console", &std::cout);

    libgui::gui_window_settings settings;
    settings.width  = 1024;
    settings.height = 768;
    settings.title  = std::string("Simple ") + libgui::get_backend_type();

    app app(settings);
    app.show();

    return 0;
}