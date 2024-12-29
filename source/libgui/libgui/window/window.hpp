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
    class window {
    public:
        using event_callback_handle_t = ev::event_system::handle_t;

        struct mouse_pos {
            double x  = 0.0;
            double y  = 0.0;
            double dx = 0.0;
            double dy = 0.0;
        };

    public:
        window(const window_settings& settings);
    	virtual ~window();

    public:
        // Get window settings
        window_settings get_settings() const;

        // Set window settings
        void set_settings(const window_settings& settings);

    public:
        // Start the event loop
        // This is a blocking function until the window is closed
        void event_loop();

        // Close the window
        template<typename T>
        requires ev::is_event<T, ev::window_event> || ev::is_event<T, ev::user_event>
        event_callback_handle_t event_attach(std::function<void(const T&)> callback)
        {
            return m_event_system.prepend_callback<T>(callback);
        }

        template<typename T>
        requires ev::is_event<T, ev::window_event> || ev::is_event<T, ev::user_event>
        void event_detach(event_callback_handle_t handle)
        {
            m_event_system.remove_callback<T>(handle);
        }

        void close();

        // Minimize the window to taskbar
        void minimize_to_tb();

        // Minimize the window to system tray
        void minimize_to_st();

        void minimize_restore();

        // Maximize the window
        void maximize();

        // Restore the window from maximized to windowed size
        void maximize_restore();

        // Set taskbar progress bar state
        void set_tb_state(taskbar_state state);

        // Set taskbar progress bar value
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

    protected:
        mouse_pos m_mouse_pos{};

    protected:
    	virtual bool initialize();
        virtual void teardown();

        // Implement when using borderless mode and providing
        // a custom titlebar.
        virtual window_titlebar* get_titlebar();

    	virtual void on_before_update();
    	virtual void on_update();
    	virtual void on_after_update();

    	virtual void on_gui_before_update();
    	virtual void on_gui_update();
    	virtual void on_gui_after_update();

    	virtual void on_resize(int width, int height);
    	virtual void on_scroll(double dx, double dy);
    	virtual void on_mouse_button(int button, int action, int modifier);
    	virtual void on_mouse_move(double dx, double dy);
        virtual void on_minimize(bool minimized);
        virtual void on_drop(int count, const char* paths[]);

    protected:
        // Clear the frame buffer to a specific color
        void clear_frame_buffer(float r = 0x00, float g = 0x00, float b = 0x00);
    
    private:
        friend internals::window_context;

    private:
        std::unique_ptr<internals::window_context> m_context      = nullptr;
        ev::event_system                           m_event_system = {};

    private:
        bool internal_initialize_imgui();
        void internal_teardown_imgui();
    };
}
