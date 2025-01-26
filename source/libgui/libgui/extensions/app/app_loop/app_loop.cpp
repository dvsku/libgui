#include "libgui/extensions/app/app_loop/app_loop.hpp"
#include "libgui/defines/env.hpp"

#ifdef LIBGUI_OS_WINDOWS
    #include <timeapi.h>
#endif

#include <algorithm>

using namespace libgui::ex::app;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

app_loop::app_loop()
    : m_context(1), m_co_cv(m_context)
{
#ifdef LIBGUI_OS_WINDOWS
    timeBeginPeriod(1);
#endif
}

app_loop::~app_loop() {
#ifdef LIBGUI_OS_WINDOWS
    timeEndPeriod(1);
#endif
}

void app_loop::start() {
    m_context.reset();
    
    for (auto& context : m_timer_contexts) {
        using namespace asio::experimental::awaitable_operators;

        asio::co_spawn(m_context, internal_co_timer(context) || m_co_cv.wait(), [](auto, auto) {});
    }

    m_context.run();
}

void app_loop::stop() {
    if (!m_context.stopped()) {
        m_context.stop();
    }

    m_co_cv.notify_all();
    m_co_cv.wait_for_end();
}

void app_loop::update(uint64_t interval, timer_callback_t callback) {
    if (!callback) {
        return;
    }

    m_timer_contexts.push_back({});
    m_timer_contexts.back().interval = std::chrono::milliseconds(interval);
    m_timer_contexts.back().callback = callback;
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

asio::awaitable<void> app_loop::internal_co_timer(internals::timer_context& context) {
    using timer_t     = asio::steady_timer;
    using timepoint_t = timer_t::time_point;
    using clock_t     = timer_t::clock_type;

    timer_t     timer(m_context, context.interval);
    timer_event timer_event;

    // TP 1/2 : Wait time
    // TP 2/3 : Execution time

    timepoint_t tp_1 = clock_t::now();
    timepoint_t tp_2 = tp_1;
    timepoint_t tp_3 = tp_1;

    while (true) {
        // Set timer to expire to requested interval minus the execution time.
        timer.expires_from_now(std::chrono::duration_cast<std::chrono::milliseconds>(context.interval - (tp_3 - tp_2)));
        
        // Set delta to previous execution time.
        timer_event.delta = std::chrono::duration_cast<std::chrono::milliseconds>(tp_3 - tp_2);

        tp_1 = clock_t::now();

        // Wait.
        auto [ec] = co_await timer.async_wait(asio::as_tuple(asio::use_awaitable));

        if (ec) {
            break;
        }

        tp_2 = clock_t::now();

        // Add wait time to execution time.
        timer_event.delta += std::chrono::duration_cast<std::chrono::milliseconds>(tp_2 - tp_1);

        if (context.callback) {
            context.callback(timer_event);
        }

        if (timer_event.cancel) {
            break;
        }

        tp_3 = clock_t::now();
    }
}
