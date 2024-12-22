#include "libgui/misc/version.hpp"

using namespace libgui;

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

static std::string internal_packed_to_string(uint64_t packed);

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

version::version(uint64_t packed) {
    m_packed_ver = packed;
    m_str_ver    = internal_packed_to_string(m_packed_ver);
}

version::version(uint16_t major, uint16_t minor, uint16_t rev) {
    m_packed_ver |= static_cast<uint64_t>(major) << 48;
    m_packed_ver |= static_cast<uint64_t>(minor) << 32;
    m_packed_ver |= static_cast<uint64_t>(rev)   << 16;

    m_str_ver = internal_packed_to_string(m_packed_ver);
}

version::version(const std::string& major, const std::string& minor, const std::string& rev) {
    uint16_t l_major = static_cast<uint16_t>(std::stoul(major.c_str()));
    uint16_t l_minor = static_cast<uint16_t>(std::stoul(minor.c_str()));
    uint16_t l_rev   = static_cast<uint16_t>(std::stoul(rev.c_str()));
    
    m_packed_ver |= static_cast<uint64_t>(l_major) << 48;
    m_packed_ver |= static_cast<uint64_t>(l_minor) << 32;
    m_packed_ver |= static_cast<uint64_t>(l_rev)   << 16;

    m_str_ver = internal_packed_to_string(m_packed_ver);
}

uint64_t version::get_packed() const {
    return m_packed_ver;
}

uint16_t version::get_major() const {
    return static_cast<uint16_t>((m_packed_ver >> 48) & 0xFFFF);
}

uint16_t version::get_minor() const {
    return static_cast<uint16_t>((m_packed_ver >> 32) & 0xFFFF);
}

uint16_t version::get_rev() const {
    return static_cast<uint16_t>((m_packed_ver >> 16) & 0xFFFF);
}

const std::string& version::to_string() const {
    return m_str_ver;
}

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

std::string internal_packed_to_string(uint64_t packed) {
    uint16_t l_major = static_cast<uint16_t>((packed >> 48) & 0xFFFF);
    uint16_t l_minor = static_cast<uint16_t>((packed >> 32) & 0xFFFF);
    uint16_t l_rev   = static_cast<uint16_t>((packed >> 16) & 0xFFFF);

    return std::string(std::to_string(l_major) + "." + std::to_string(l_minor) + "." + std::to_string(l_rev));
}
