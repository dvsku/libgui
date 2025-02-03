#pragma once

#include "libgui/defines/env.hpp"

#ifdef LIBGUI_OS_WINDOWS
    #ifndef _WINDOWS_
        #ifndef NOMINMAX
            #define NOMINMAX
        #endif

        #ifndef WIN32_LEAN_AND_MEAN
            #define WIN32_LEAN_AND_MEAN
        #endif

        #include <windows.h>
    #endif
#endif
