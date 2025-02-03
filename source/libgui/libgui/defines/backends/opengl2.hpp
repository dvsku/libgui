#pragma once

#include "libgui/defines/env.hpp"
#include "libgui/defines/os.hpp"

#ifdef LIBGUI_OS_WINDOWS
    #ifndef __GL_H__
        #include <gl/gl.h>
    #endif
#endif
