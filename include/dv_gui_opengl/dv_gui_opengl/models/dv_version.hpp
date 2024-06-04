#pragma once

#include <cstdint>
#include <string>

namespace dvsku {
    class dv_version {
    public:
        dv_version() = default;

        dv_version(uint64_t packed);
        dv_version(uint16_t major, uint16_t minor, uint16_t rev);
        dv_version(const std::string& major, const std::string& minor, const std::string& rev);

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