#pragma once

#define STRICT_TYPED_ITEMIDS

#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <cstdint>

#include "libgui/defines/backend.hpp"
#include "libgui/defines/imgui.hpp"
#include "libgui/defines/windowing.hpp"
#include "libgui/window/window_taskbar.hpp"

struct GLFWwindow;

namespace libgui::internals {
    class window_context {
    public:
        window_context() = delete;
        window_context(GLFWwindow* handle);
        ~window_context();

    public:
        bool initialize();
        void teardown();

        // Get handle to GLFW window instance
        GLFWwindow* get_glfw_handle();

        // Get handle to Win32 window instance
        HWND get_win32_handle();

    public:
        // Request for the window to be closed
        void request_close();

        // Minimize the window to taskbar
        void minimize_to_tb();

        // Minimize the window to system tray
        void minimize_to_st();

        // Set window maximized or restore to windowed size
        void set_maximized(bool value);

        // Change window style to borderless
        // Handles low level Win32 changes
        // Return true if change was successful, false if no change occurred
        bool set_borderless(bool value);

        // Set window to be resizable or a fixed size
        // When set to a fixed size, the window is locked to the current size
        // Return true if change was successful, false if no change occurred
        bool set_resizable(bool value);

        // Set taskbar progress bar state
        void set_tb_state(taskbar_state state);

        // Set taskbar progress bar value
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

        // Is borderless style set?
        bool is_borderless() const;

        // Is window resizable?
        bool is_resizable() const;

    private:
        // Handle to GLFW window instance
        GLFWwindow* m_glfw_handle = nullptr;

        // Default WNDPROC callback
        WNDPROC m_wndproc_default_callback = nullptr;

        // Standard Win32 window style
        LONG_PTR m_standard_wnd_style = 0;

        // Borderless Win32 window style
        LONG_PTR m_borderless_wnd_style = 0;

        // Handle to system tray data for this window instance
        NOTIFYICONDATA* m_st_handle = nullptr;

        // Is system tray icon visible?
        bool m_st_icon_visible = false;

        // Handle to taskbar for this window instance
        ITaskbarList3* m_tb_handle = nullptr;

        // Is minimized to system tray?
        bool m_minimized_to_st = false;

        // Is window borderless?
        bool m_borderless = false;

        // Is window resizable?
        bool m_resizable = false;

    private:
        void internal_initialize_st();
        void internal_teardown_st();

        void internal_initialize_tb();
        void internal_teardown_tb();

        // Show/hide window icon in system tray
        // Return true if change was successful, false if no change occurred
        bool internal_set_st_icon_visible(bool value);

        // Restore window from system tray
        // Called by wndproc_callback_*()
        void internal_restore_from_st();

    private:
        static LRESULT __stdcall internal_wndproc_callback_standard(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
        static LRESULT __stdcall internal_wndproc_callback_borderless(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

    private:
        static void internal_resize_callback(GLFWwindow* window, int width, int height);
        static void internal_scroll_callback(GLFWwindow* window, double dx, double dy);
        static void internal_mouse_button_callback(GLFWwindow* window, int button, int action, int modifier);
        static void internal_mouse_move_callback(GLFWwindow* window, double x, double y);
        static void internal_minimize_callback(GLFWwindow* window, int minimized);
        static void internal_drop_callback(GLFWwindow* window, int count, const char* paths[]);
    };
}
