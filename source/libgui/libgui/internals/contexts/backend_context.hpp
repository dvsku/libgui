#pragma once

#include "libgui/internals/defines/libgui.hpp"
#include "libgui/window/window_base.hpp"

namespace libgui::internals {
    class backend_context {
    public:
        backend_context(libgui::window_base* wnd, internals::window_base* wnd_internal);

    public:
        // Initialize context.
        virtual result initialize(const window_settings& settings,
            const window_startup_settings& startup_settings) = 0;

        // Teardown context.
        virtual void teardown() = 0;

    public:
        // Get window settings.
        window_settings* get_settings();

        // Set window settings.
        virtual void set_settings(const window_settings& settings) = 0;

    public:
        // Poll for events.
        virtual void event_poll() = 0;

        // Wait for events.
        virtual void event_wait() = 0;

    public:
        void close(bool force);

        // Set window minimized to taskbar or restore from taskbar.
        void set_minimize_to_tb(bool value);

        // Set window minimized to system tray or restore from system tray.
        void set_minimize_to_st(bool value);

        // Set window maximized or restore to windowed mode.
        void set_maximized(bool value);

        // Set taskbar progress bar state.
        virtual void set_tb_state(taskbar_state state);

        // Set taskbar progress bar value.
        virtual void set_tb_value(uint64_t value);

    public:
        // Is window set to close?
        virtual bool is_closing() const = 0;

        // Is window minimized to taskbar?
        bool is_minimized_to_tb() const;

        // Is window minimized to system tray?
        bool is_minimized_to_st() const;

        // Is window maximized?
        bool is_maximized() const;

        // Is window resizable?
        bool is_resizable() const;

    public:
        // Prepare for incoming draw instructions.
        virtual void prepare_for_drawing();

        // Draw.
        virtual void draw();

        // Clear window to specific color.
        virtual void clear(float r, float g, float b);

    protected:
        // Handles to libgui window instance.
        libgui::window_base*    m_wnd          = nullptr;
        internals::window_base* m_wnd_internal = nullptr;

        // Window settings.
        window_settings m_settings = {};

        // Window state.
        flags_t m_state = 0;

    protected:
        template<typename T>
        requires libgui::ev::is_event<T, libgui::ev::event>
        void internal_event_enqueue(T&& event) {
            if (m_wnd_internal) {
                m_wnd_internal->m_event_system.enqueue<T>(std::move(event));
            }
        }

        template<typename T>
        requires libgui::ev::is_event<T, libgui::ev::event>
        void internal_event_dispatch(T&& event) {
            if (m_wnd_internal) {
                m_wnd_internal->m_event_system.dispatch<T>(std::move(event));
            } 
        }

        template<typename T>
        requires libgui::ev::is_event<T, libgui::ev::event>
        libgui::window_base::event_callback_handle_t internal_event_attach(std::function<void(const T&)> callback) {
            if (m_wnd_internal) {
                return m_wnd_internal->m_event_system.append_callback<T>(callback);
            }

            return {};
        }
    };
}
