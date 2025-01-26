#include "libgui/extensions/app/app_loop/internals/coroutine_cv.hpp"

using namespace libgui::ex::app::internals;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

coroutine_cv::coroutine_cv(io_context_t& context) 
    : m_io_context(context), m_timer(context, asio::steady_timer::duration::max()) {}

asio::awaitable<void> coroutine_cv::wait() {
    m_operations++;
    co_await m_timer.async_wait(asio::as_tuple(asio::use_awaitable));
    m_operations--;
    co_return;
}

void coroutine_cv::wait_for_end() {
    while (true) {
        if (m_operations == 0 || m_io_context.stopped())
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

bool coroutine_cv::is_waiting() {
    return m_operations != 0;
}

void coroutine_cv::notify_one() {
    m_timer.cancel_one();
}

void coroutine_cv::notify_all() {
    m_timer.cancel();
}
