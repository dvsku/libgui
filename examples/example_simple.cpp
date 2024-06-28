#include <libgui.hpp>
#include <iostream>

class app : public libgui::window {
public:
    app(const libgui::window_settings& settings)
        : libgui::window(settings) {}
};

int main() {
    libutil::log::settings log_settings;
    log_settings.log_to_file = false;
    log_settings.level       = libutil::log::level::debug;

    libutil::log::init(log_settings);

    libgui::window_settings settings;
    settings.width  = 1024;
    settings.height = 768;
    settings.title  = std::string("Simple ") + libgui::debug::get_backend_type();

    app app(settings);
    app.show();

    return 0;
}