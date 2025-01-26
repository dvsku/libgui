#pragma once

#include "libgui/window/window_settings.hpp"
#include "libgui/window/window_taskbar.hpp"
#include "libgui/window/window_titlebar.hpp"
#include "libgui/event/event_system.hpp"
#include "libgui/event/events.hpp"

#include <memory>

namespace libgui::internals {
    class window_context;
}

namespace libgui {
    class window_base {
    public:
        using event_callback_handle_t = ev::event_system::handle_t;

    public:
        wtb_flags_t tb_flags = 0;

    public:
        // Get window settings.
        window_settings get_settings() const;

        // Set window settings.
        void set_settings(const window_settings& settings);

    public:
        // Request to close the window.
        // Does invoke ev_close event.
        // 
        // Provides the same behavior as the close (X) button.
        void close();

        // Minimize the window to taskbar.
        void minimize_to_tb();

        // Minimize the window to system tray.
        void minimize_to_st();

        // Restore window from minimized state.
        // Restores from taskbar and system tray.
        void minimize_restore();

        // Maximize the window.
        // Ignored if window is not resizable.
        void maximize();

        // Restore the window from maximized to windowed mode.
        void maximize_restore();

        // Set taskbar progress bar state.
        void set_tb_state(taskbar_state state);

        // Set taskbar progress bar value.
        void set_tb_value(uint64_t value);

    public:
        // Is window minimized to taskbar or system tray?
        bool is_minimized() const;

        // Is window minimized to taskbar?
        bool is_minimized_to_tb() const;

        // Is window minimized to system tray?
        bool is_minimized_to_st() const;

        // Is window maximized?
        bool is_maximized() const;

    public:
        void event_poll();

        template<typename T>
        requires ev::is_event<T, ev::window_event> || ev::is_event<T, ev::user_event>
        event_callback_handle_t event_attach(std::function<void(const T&)> callback) {
            return m_event_system.prepend_callback<T>(callback);
        }

        template<typename T>
        requires ev::is_event<T, ev::window_event> || ev::is_event<T, ev::user_event>
        void event_detach(event_callback_handle_t handle) {
            m_event_system.remove_callback<T>(handle);
        }

    public:
        // Clear window to specific color.
        void clear(float r = 0x00, float g = 0x00, float b = 0x00);

    public:
        // Prepare window backend for incoming draw instructions.
        virtual void prepare_for_drawing() = 0;

        // Execute draw instructions.
        virtual void draw() = 0;

    protected:
        friend internals::window_context;

    protected:
        std::unique_ptr<internals::window_context> m_context      = nullptr;
        ev::event_system                           m_event_system = {};
    };
}
