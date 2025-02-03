#pragma once

#include "libgui/defines/os.hpp"
#include "libgui/defines/backends/glfw.hpp"
#include "libgui/event/events.hpp"
#include "libgui/event/events_internal.hpp"
#include "libgui/internals/contexts/backend_context.hpp"

#define STRICT_TYPED_ITEMIDS

#include <shellapi.h>
#include <shlobj.h>

namespace libgui::internals {
    class backend_context_glfw : public backend_context {
    public:
        backend_context_glfw(libgui::window_base* wnd, internals::window_base* wnd_internal)
            : backend_context(wnd, wnd_internal) {}

    public:
        void set_settings(const window_settings& settings) override;

    public:
        void event_poll() override;
        void event_wait() override;

    public:
        // Set taskbar progress bar state.
        void set_tb_state(taskbar_state state) override;

        // Set taskbar progress bar value.
        void set_tb_value(uint64_t value) override;

    public:
        bool is_closing() const override;

    protected:
        // Handle to GLFW window instance.
        GLFWwindow* m_glfw_handle = nullptr;

    protected:
        // Initialize context.
        virtual libgui::result initialize(const window_settings& settings,
            const window_startup_settings& startup_settings) override;

        // Teardown context.
        virtual void teardown() override;

    private:
        // Default WNDPROC callback.
        WNDPROC m_default_wndproc_callback = nullptr;

        // Handle to system tray data for this window instance.
        NOTIFYICONDATA* m_st_handle = nullptr;

        // Handle to taskbar for this window instance.
        ITaskbarList3* m_tb_handle = nullptr;

        // Window minimum size.
        libgui::size_i m_minimum_size = { -1, -1 };

        // Window maximum size.
        libgui::size_i m_maximum_size = { -1, -1 };

    private:
        // Get handle to Win32 window instance.
        HWND get_win32_handle();

    private:
        libgui::result internal_initialize_glfw();
        void           internal_teardown_glfw();

        libgui::result internal_initialize_win32();
        void           internal_teardown_win32();

        void internal_initialize_event_handlers();
        void internal_teardown_event_handlers();

        void internal_initialize_tb();
        void internal_teardown_tb();

        void internal_initialize_st();
        void internal_teardown_st();

    private:
        // Show/hide window icon in system tray.
        // Return true if change was successful, false if no change occurred.
        bool internal_set_st_icon_visible(bool value);

    private:
        static LRESULT __stdcall internal_wndproc_callback(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

        LRESULT internal_wndproc_wmsyscommand(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
        LRESULT internal_wndproc_wmsize(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
        LRESULT internal_wndproc_wmshowwindow(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
        LRESULT internal_wndproc_wmusertrayicon(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
        LRESULT internal_wndproc_wmnccalcsize(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
        LRESULT internal_wndproc_wmnchittest(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
        LRESULT internal_wndproc_wmgetminmaxinfo(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
        LRESULT internal_wndproc_wmnclbuttondown(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
        LRESULT internal_wndproc_wmnclbuttonup(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

    private:
        void internal_event_callback(const internals::ev::ev_close& event);

    private:
        void internal_event_callback(const libgui::ev::ev_close& event);
        void internal_event_callback(const libgui::ev::ev_maximize& event);
        void internal_event_callback(const libgui::ev::ev_minimize_to_tb& event);
        void internal_event_callback(const libgui::ev::ev_minimize_to_st& event);
        void internal_event_callback(const libgui::ev::ev_maximize_restore& event);
        void internal_event_callback(const libgui::ev::ev_minimize_restore_from_tb& event);
        void internal_event_callback(const libgui::ev::ev_minimize_restore_from_st& event);
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
