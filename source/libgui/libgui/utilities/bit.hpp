#pragma once

#define LIBGUI_SET_FLAG(flags, flag, value)     \
    (value ? (flags |= (flag)) : (flags &= ~(flag)))

#define LIBGUI_HAS_FLAG(flags, flag)            \
    ((flags & (flag)) != 0)
