#pragma once

#include <cstdint>
#include <string>

namespace libgui {
    class version {
    public:
        version() = default;

        version(uint64_t packed);
        version(uint16_t major, uint16_t minor, uint16_t rev);
        version(const std::string& major, const std::string& minor, const std::string& rev);

    public:
        uint64_t get_packed() const;
        uint16_t get_major()  const;
        uint16_t get_minor()  const;
        uint16_t get_rev()    const;

        const std::string& to_string() const;

    private:
        uint64_t    m_packed_ver = 0U;
        std::string m_str_ver    = "";
    };
}