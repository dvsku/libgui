#pragma once

#include "libgui/models/gui_window_settings.hpp"
#include "libgui/models/gui_window_title_bar.hpp"
#include "libgui/models/gui_taskbar_status.hpp"

#include <string>

namespace libgui {
    class gui_window_context;

    class gui_window {
    public:
        using handle_t = void*;
        using ptr_t    = intptr_t*;

        struct mouse_pos {
            double x  = 0.0;
            double y  = 0.0;
            double dx = 0.0;
            double dy = 0.0;
        };

    public:
        gui_window(const gui_window_settings& settings);
    	virtual ~gui_window();

    public:
        // Get custom title bar hit testing flags
        gui_window_title_bar& get_title_bar();

        // Get initial window settings
        const gui_window_settings& get_settings() const;

        // Set taskbar progress bar status.
        void set_taskbar_status(gui_taskbar_status status);

        // Set taskbar progress bar value.
        void set_taskbar_progress(uint64_t progress);

    public:
        // Show the window
        void show();

        // Close the window
        void close();

        // Iconify the window to taskbar
        void iconify();

        // Fullscreen or restore original size
        void maximize_or_restore();

        // Is window iconified to taskbar
        bool is_iconified() const;

        // Is window fullscreen
        bool is_maximized() const;

    protected:
        ptr_t m_native    = nullptr;
        bool  m_resizable = false;

        gui_window_title_bar m_title_bar{};
        mouse_pos           m_mouse_pos{};

    protected:
    	virtual bool prepare();
        virtual void release();

    	virtual void on_before_update();
    	virtual void on_update();
    	virtual void on_after_update();

    	virtual void on_gui_before_update();
    	virtual void on_gui_update();
    	virtual void on_gui_after_update();

    	virtual void on_resize(int width, int height);
    	virtual void on_scroll(double dx, double dy);
    	virtual void on_mouse_button(int btn, int action, int modifier);
    	virtual void on_mouse_move(double dx, double dy);
        virtual void on_drop(int count, const char* paths[]);

    protected:
        // Set borderless
        void set_borderless();

        // Set resizable flag.
        // If false, locks the window to the current size.
        void set_resizable(bool value);

        // Clear the frame buffer to a specific color
        void clear_frame_buffer(float r = 0x00, float g = 0x00, float b = 0x00, float a = 0xFF);

    private:
        /*
            Default callback used by the underlying windowing system.
            Ignored when the window doesn't have a custom title bar.
        */
        ptr_t m_default_callback = nullptr;

        /*
            Underlying system impl of the taskbar element for this window.
        */
        ptr_t m_taskbar = nullptr;

    private:
        gui_window_settings m_settings;

    private:
        friend gui_window_context;

    private:
    	void loop();

        void prepare_taskbar();
        void release_taskbar();
    };
}