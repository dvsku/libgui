#pragma once

#include <cstdint>
#include <string>

namespace dvsku {
    class texture {
    public:
        uint32_t id         = 0U;
        int      width      = 0U;
        int      height     = 0U;
        int      components = 0U;

    public:
        texture()                  = delete;
        texture(const texture&) = delete;
        texture(texture&&)      = delete;

        texture(const std::string& filename);
        texture(const char* ptr, size_t size);

        ~texture();

        texture& operator=(const texture&) = delete;
        texture& operator=(texture&&)      = delete;
    };
}