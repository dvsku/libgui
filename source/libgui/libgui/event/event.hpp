#pragma once

#include <memory>

namespace libgui::ev {
    class event {
    public:
        using ptr_t = std::shared_ptr<event>;

    public:
        mutable bool cancel = false;
    };

    template<typename T, typename Base>
    concept is_event = !std::same_as<T, Base> && std::derived_from<T, Base>;

    class event_type {
    private:
        inline static uint64_t m_cnt = 0U;

    public:
        template<typename T>
        requires is_event<T, event>
        static const uint64_t id = m_cnt++;
    };
}
