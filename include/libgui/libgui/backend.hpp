#pragma once

namespace libgui {
    class backend {
    public:
        backend();
        ~backend();

    public:
        static void initialize();
        static void teardown();

    private:
        inline static bool m_is_initialized = false;
    };
}
