#include "libgui/internals/utilities/win32.hpp"

using namespace libgui::internals;

LONG_PTR win32::get_window_default_style() {
    LONG_PTR style = 0;
    style |= WS_VISIBLE;
    style |= WS_CLIPSIBLINGS;
    style |= WS_CLIPCHILDREN;
    style |= WS_BORDER;
    style |= WS_DLGFRAME;
    style |= WS_SYSMENU;
    style |= WS_THICKFRAME;
    style |= WS_MINIMIZEBOX;
    style |= WS_MAXIMIZEBOX;
    
    return style;
}

float win32::get_window_dpi_scaling(HWND handle) {
    UINT dpi = GetDpiForWindow(handle);
    return (float)dpi / (float)USER_DEFAULT_SCREEN_DPI;
}

libgui::rect_i win32::get_window_nc_size(HWND handle, bool maximized) {
    libgui::rect_i rect = {};
    
    if (!handle) {
        return rect;
    }

    int32_t nc_width  = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
    int32_t nc_height = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);

    rect.left   = nc_width;
    rect.right  = nc_width;
    rect.top    = maximized ? nc_height : 1;
    rect.bottom = nc_height;
    
    return rect.scale(get_window_dpi_scaling(handle));
}

bool win32::set_window_size(HWND handle, libgui::size_i size) {
    return SetWindowPos(handle, NULL, 0, 0, size.width, size.height, SWP_NOMOVE | SWP_NOZORDER);
}

bool win32::set_window_pos_relative(HWND handle, libgui::point_f pos, libgui::point_f origin) {
    if (!handle) {
        return false;
    }
    
    RECT rect = {};
    if (!GetWindowRect(handle, &rect)) {
        return false;
    }

    auto monitor = MonitorFromWindow(handle, MONITOR_DEFAULTTONEAREST);
    if (!monitor) {
        return false;
    }

    MONITORINFO monitor_info = {};
    monitor_info.cbSize = sizeof(MONITORINFO);

    if (!GetMonitorInfo(monitor, &monitor_info)) {
        return false;
    }

    libgui::point_i new_pos = {};
    new_pos.x = (int32_t)(((float)(monitor_info.rcWork.right - monitor_info.rcWork.left) * pos.x) - ((float)(rect.right - rect.left) * origin.x));
    new_pos.y = (int32_t)(((float)(monitor_info.rcWork.bottom - monitor_info.rcWork.top) * pos.y) - ((float)(rect.bottom - rect.top) * origin.y));
    
    return SetWindowPos(handle, NULL, new_pos.x, new_pos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
