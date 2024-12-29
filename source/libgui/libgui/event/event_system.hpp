#pragma once

#include "libgui/event/event.hpp"

#include <eventpp/eventqueue.h>
#include <cstdint>

namespace libgui::ev {
    class event_system {
    public:
        using event_t = libgui::ev::event;

        struct event_system_policies {
            static bool canContinueInvoking(const event_t::ptr_t event) {
                return event && !event->cancel;
            }
        };

        using event_queue_t = eventpp::EventQueue<uint64_t, void(const event_t::ptr_t), event_system_policies>;
        using handle_t      = event_queue_t::Handle;

    public:
        template<typename T>
        requires is_event<T, event_t>
        void enqueue(T&& event) {
            if (has_callback<T>())
                m_queue.enqueue(event_type::id<T>, std::make_shared<T>(std::move(event)));
        }

        template<typename T>
        requires is_event<T, event_t>
        void dispatch(T&& event) {
            if (has_callback<T>())
                m_queue.dispatch(event_type::id<T>, std::make_shared<T>(std::move(event)));
        }

        template<typename T>
        requires is_event<T, event_t>
        handle_t append_callback(std::function<void(const T&)> callback) {
            return m_queue.appendListener(event_type::id<T>, [callback](const event_t::ptr_t event) {
                callback(*std::static_pointer_cast<T>(event));
            });
        }

        template<typename T>
        requires is_event<T, event_t>
        handle_t prepend_callback(std::function<void(const T&)> callback) {
            return m_queue.prependListener(event_type::id<T>, [callback](const event_t::ptr_t event) {
                callback(*std::static_pointer_cast<T>(event));
            });
        }

        template<typename T>
        requires is_event<T, event_t>
        void remove_callback(handle_t handle) {
            m_queue.removeListener(event_type::id<T>, handle);
        }

        template<typename T>
        requires is_event<T, event_t>
        bool has_callback() {
            return m_queue.hasAnyListener((event_type::id<T>));
        }

        void process() {
            m_queue.process();
        }

        void process_one() {
            m_queue.processOne();
        }

    private:
        event_queue_t m_queue;
    };
}
