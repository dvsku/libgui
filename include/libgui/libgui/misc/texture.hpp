#pragma once

#include <cstdint>
#include <string>

namespace dvsku {
    class dv_texture {
    public:
        uint32_t id         = 0U;
        int      width      = 0U;
        int      height     = 0U;
        int      components = 0U;

    public:
        dv_texture()                  = delete;
        dv_texture(const dv_texture&) = delete;
        dv_texture(dv_texture&&)      = delete;

        dv_texture(const std::string& filename);
        dv_texture(const char* ptr, size_t size);

        ~dv_texture();

        dv_texture& operator=(const dv_texture&) = delete;
        dv_texture& operator=(dv_texture&&)      = delete;
    };
}