#pragma once

#include <atomic>

namespace libgui {
    class window;

    /*
        A class to track async work progress.
    */
    class work_context {
    public:
        std::atomic_bool cancel = false;

    public:
        work_context(window& window)
            : m_window(window) {}

    public:
        /*
            Is there any work being preformed?
        */
        bool is_working() const;

        /*
            Work started.
        */
        void start_working();

        /*
            Work stopped.
        */
        void stop_working(bool success_or_cancel);

    public:
        float get_progress() const;
        float get_subprogress() const;

        void set_progress(float progress);
        void set_subprogress(float subprogress);

        void update_progress(float change);
        void update_subprogress(float change);

    private:
        window& m_window;

        std::atomic_bool   m_working     = false;
        std::atomic<float> m_progress    = 0.0f;
        std::atomic<float> m_subprogress = 0.0f;
    };
}
