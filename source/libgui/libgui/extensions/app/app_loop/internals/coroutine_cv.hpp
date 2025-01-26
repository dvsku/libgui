#pragma once

#include "libgui/extensions/app/app_loop/defines/asio.hpp"

#include <atomic>

namespace libgui::ex::app::internals {
    class coroutine_cv {
    public:
        using io_context_t = asio::io_context;

    public:
        coroutine_cv()                    = delete;
        coroutine_cv(const coroutine_cv&) = delete;
        coroutine_cv(coroutine_cv&&)      = default;

        coroutine_cv(io_context_t& context);

        coroutine_cv& operator=(const coroutine_cv&) = delete;
        coroutine_cv& operator=(coroutine_cv&&)      = default;

    public:
        // Wait for notify or expire.
        asio::awaitable<void> wait();
        
        // Wait for all operations to finish.
        void wait_for_end();

        // Is anything waiting on this cv?
        bool is_waiting();

        void notify_one();
        void notify_all();

    private:
        io_context_t&        m_io_context;
        asio::steady_timer   m_timer;
        std::atomic_uint16_t m_operations = 0U;
    };
}
