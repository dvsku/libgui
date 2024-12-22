#pragma once

#include <string>

namespace libgui {
    class backend {
    public:
        backend();
        ~backend();

    public:
        void initialize();
        void teardown();

        const char* get_type();
        const char* get_version();

    private:
        inline static bool        m_is_initialized = false;
        inline static std::string m_version        = "";
    };
}
