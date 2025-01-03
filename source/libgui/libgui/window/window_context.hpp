#pragma once

#define STRICT_TYPED_ITEMIDS

#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>

#include "libgui/defines/backend.hpp"
#include "libgui/defines/imgui.hpp"
#include "libgui/defines/windowing.hpp"
#include "libgui/defines/libgui.hpp"
#include "libgui/window/window.hpp"
#include "libgui/event/events_internal.hpp"

// Is minimized to taskbar?
#define LIBGUI_WND_STATE_MINIMIZED_TO_TB 1 << 0

// Is minimized to system tray?
#define LIBGUI_WND_STATE_MINIMIZED_TO_ST 1 << 1

// Is system tray icon visible?
#define LIBGUI_WND_STATE_ST_ICON_VISIBLE 1 << 2

// Is window maximized?
#define LIBGUI_WND_STATE_MAXIMIZED 1 << 3

#define LIBGUI_WND_STATE_RESERVED_1 1 << 4
#define LIBGUI_WND_STATE_RESERVED_2 1 << 5

// Is window resizable?
#define LIBGUI_WND_STATE_RESIZABLE 1 << 6

struct GLFWwindow;

namespace libgui::internals {
    class window_context {
    public:
        window_context() = delete;

        window_context(window* window, const window_settings& settings,
            const window_startup_settings& startup_settings);

        ~window_context();

    public:
        bool initialize(const window_startup_settings& startup_settings);
        void teardown();

        // Get handle to GLFW window instance.
        GLFWwindow* get_glfw_handle();

        // Get handle to Win32 window instance.
        HWND get_win32_handle();

        // Get window settings.
        window_settings* get_settings();

        // Set window settings.
        void set_settings(const window_settings& settings);

    public:
        // Poll for events.
        void event_poll();

        // Wait for events.
        void event_wait();

    public:
        // Request for the window to be closed.
        void request_close();

        // Set window minimized to taskbar or restore from taskbar.
        void set_minimize_to_tb(bool value);

        // Set window minimized to system tray or restore from system tray.
        void set_minimize_to_st(bool value);

        // Set window maximized or restore to windowed mode.
        void set_maximized(bool value);

        // Set window to be resizable or a fixed size.
        // When set to a fixed size, the window is locked to the current size.
        // Return true if change was successful, false if no change occurred.
        bool set_resizable(bool value);

        // Set taskbar progress bar state.
        void set_tb_state(taskbar_state state);

        // Set taskbar progress bar value.
        void set_tb_value(uint64_t value);

    public:
        // Is window set to close?
        bool is_closing() const;

        // Is window minimized to taskbar?
        bool is_minimized_to_tb() const;

        // Is window minimized to system tray?
        bool is_minimized_to_st() const;

        // Is window maximized?
        bool is_maximized() const;

        // Is window resizable?
        bool is_resizable() const;

    private:
        // Handle to libgui window instance.
        window* m_wnd = nullptr;

        // Window settings.
        window_settings m_settings = {};

        // Handle to GLFW window instance.
        GLFWwindow* m_glfw_handle = nullptr;

        // Default WNDPROC callback.
        WNDPROC m_default_wndproc_callback = nullptr;

        // Handle to system tray data for this window instance.
        NOTIFYICONDATA* m_st_handle = nullptr;

        // Handle to taskbar for this window instance.
        ITaskbarList3* m_tb_handle = nullptr;

        // Window state.
        flags_t m_state = 0;

        // Window minimum size.
        libgui::size_i m_minimum_size = { -1, -1 };

        // Window maximum size.
        libgui::size_i m_maximum_size = { -1, -1 };

    private:
        template<typename T>
        requires libgui::ev::is_event<T, libgui::ev::event>
        void internal_event_enqueue(T&& event)
        {
            if (!m_wnd) return;
            m_wnd->m_event_system.enqueue<T>(std::move(event));
        }

        template<typename T>
        requires libgui::ev::is_event<T, libgui::ev::event>
        void internal_event_dispatch(T&& event)
        {
            if (!m_wnd) return;
            m_wnd->m_event_system.dispatch<T>(std::move(event));
        }

        template<typename T>
        requires libgui::ev::is_event<T, libgui::ev::event>
        window::event_callback_handle_t internal_event_attach(std::function<void(const T&)> callback)
        {
            if (!m_wnd) return {};
            return m_wnd->m_event_system.append_callback<T>(callback);
        }

    private:
        bool internal_initialize_win32();
        void internal_teardown_win32();
        
        bool internal_initialize_glfw();
        void internal_teardown_glfw();

        void internal_initialize_st();
        void internal_teardown_st();

        void internal_initialize_tb();
        void internal_teardown_tb();

        void internal_initialize_event();
        void internal_teardown_event();

    private:
        // Set resizable and size constraints.
        void internal_startup_set_resizable(bool value, const libgui::size_i& current, const libgui::size_i& minimum);

    private:
        // Show/hide window icon in system tray.
        // Return true if change was successful, false if no change occurred.
        bool internal_set_st_icon_visible(bool value);

    private:
        static LRESULT __stdcall internal_wndproc_callback(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

        LRESULT internal_wndproc_wmsyscommand(WPARAM wparam, LPARAM lparam);
        LRESULT internal_wndproc_wmsize(WPARAM wparam, LPARAM lparam);
        LRESULT internal_wndproc_wmshowwindow(WPARAM wparam, LPARAM lparam);
        LRESULT internal_wndproc_wmusertrayicon(WPARAM wparam, LPARAM lparam);
        LRESULT internal_wndproc_wmnccalcsize(WPARAM wparam, LPARAM lparam);
        LRESULT internal_wndproc_wmnchittest(WPARAM wparam, LPARAM lparam);
        LRESULT internal_wndproc_wmgetminmaxinfo(WPARAM wparam, LPARAM lparam);
        LRESULT internal_wndproc_wmnclbuttondown(WPARAM wparam, LPARAM lparam);
        LRESULT internal_wndproc_wmnclbuttonup(WPARAM wparam, LPARAM lparam);

    private:
        void internal_event_callback(const libgui::ev::ev_close& event);
        void internal_event_callback(const libgui::ev::ev_minimize& event);
        void internal_event_callback(const libgui::ev::ev_minimize_restore& event);
        void internal_event_callback(const libgui::ev::ev_maximize& event);
        void internal_event_callback(const libgui::ev::ev_maximize_restore& event);
        void internal_event_callback(const libgui::internals::ev::ev_restore& event);
        void internal_event_callback(const libgui::internals::ev::ev_frame_buffer_resized& event);
        void internal_event_callback(const libgui::internals::ev::ev_update_settings& event);
        void internal_event_callback(const libgui::internals::ev::ev_update_startup_settings& event);

    private:
        static void internal_glfw_window_resize_callback(GLFWwindow* window, int width, int height);
        static void internal_glfw_frame_buffer_resize_callback(GLFWwindow* window, int width, int height);
        static void internal_glfw_scroll_callback(GLFWwindow* window, double dx, double dy);
        static void internal_glfw_mouse_btn_callback(GLFWwindow* window, int btn, int state, int mod);
        static void internal_glfw_mouse_move_callback(GLFWwindow* window, double x, double y);
        static void internal_glfw_drop_callback(GLFWwindow* window, int count, const char* files[]);
    };
}
