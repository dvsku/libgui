#pragma once

#include "libgui/extensions/app/app_loop/defines/asio.hpp"
#include "libgui/extensions/app/app_loop/timer_callback.hpp"
#include "libgui/extensions/app/app_loop/internals/timer_context.hpp"
#include "libgui/extensions/app/app_loop/internals/coroutine_cv.hpp"

#include <vector>

namespace libgui::ex::app {
    class app_loop {
    public:
        app_loop();
        virtual ~app_loop();

    public:
        void start();
        void stop();

    public:
        void update(uint64_t interval, timer_callback_t callback);

    private:
        using io_context_t = asio::io_context;

    private:
        io_context_t                          m_context;
        std::vector<internals::timer_context> m_timer_contexts;
        internals::coroutine_cv               m_co_cv;

    private:
        asio::awaitable<void> internal_co_timer(internals::timer_context& context);
    };
}
