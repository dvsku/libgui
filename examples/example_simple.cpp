#include <libgui.hpp>
#include <iostream>

class app : public dvsku::gui_window {
public:
    app(const dvsku::gui_window_settings& settings)
        : dvsku::gui_window(settings) {}
};

int main() {
    dvsku::util_log::settings log_settings;
    log_settings.log_to_file = false;
    log_settings.level       = dvsku::util_log::level::debug;

    dvsku::util_log::init(log_settings);
    dvsku::util_log::create_source("console", &std::cout);

    dvsku::gui_window_settings settings;
    settings.width  = 1024;
    settings.height = 768;
    settings.title  = std::string("Simple ") + dvsku::get_backend_type();

    app app(settings);
    app.show();

    return 0;
}