#pragma once

#if defined(_WIN32) || defined(WIN32)
    #define LIBGUI_OS_WINDOWS
#else
    #define LIBGUI_OS_UNIX
#endif

#if !defined(NDEBUG)
    #define LIBGUI_DEBUG
#else
    #define LIBGUI_RELEASE
#endif
