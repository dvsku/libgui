#include "libgui/fonts/font.hpp"

#include <Windows.h>
#include <filesystem>
#include <stdexcept>

using namespace libgui;

////////////////////////////////////////////////////////////////////////////////
// INTERNAL

static std::string get_windows_fonts_dir();

////////////////////////////////////////////////////////////////////////////////
// PUBLIC

font_source::font_source(const void* data, size_t size, const std::vector<font_range>& range, const font_config& config) {
    this->data  = (uint8_t*)data;
    this->size  = size;
    set_range(range);
    this->config = config;
}

font_source::font_source(const std::string& ttf_file, const std::vector<font_range>& range, const font_config& config) {
    namespace fs = std::filesystem;
    
    set_range(range);
    this->config = config;

    fs::path file = ttf_file;

    if (!file.has_extension()) {
        file.replace_extension(".ttf");
    }
        
    if (fs::exists(file)) {
        filename = file.string();
        return;
    }

    file = get_windows_fonts_dir() + file.filename().string();

    if (fs::exists(file)) {
        filename = file.string();
        return;
    }

    throw std::runtime_error("Failed to find font file.");
}

////////////////////////////////////////////////////////////////////////////////
// PRIVATE

void font_source::set_range(const std::vector<font_range>& range) {
    if (range.size() == 0) {
        this->range.push_back({ 0x0020, 0x00FF });
    }
    else {
        this->range = range;
    }

    this->range.push_back({ 0, 0 });
}

////////////////////////////////////////////////////////////////////////////////
// INTERNAL

std::string get_windows_fonts_dir() {
    char path[MAX_PATH];

    return GetWindowsDirectoryA(path, MAX_PATH) ? std::string(path) + "\\Fonts\\" : "";
}
