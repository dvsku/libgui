#include <libgui.hpp>
#include <iostream>

class app : public libgui::window {
public:
    app(const libgui::window_settings& settings)
        : libgui::window(settings) {}
};

int main() {
    dvsku::util_log::settings log_settings;
    log_settings.log_to_file = false;
    log_settings.level       = dvsku::util_log::level::debug;

    dvsku::util_log::init(log_settings);
    dvsku::util_log::create_source("console", &std::cout);

    libgui::window_settings settings;
    settings.width  = 1024;
    settings.height = 768;
    settings.title  = std::string("Simple ") + libgui::debug::get_backend_type();

    app app(settings);
    app.show();

    return 0;
}