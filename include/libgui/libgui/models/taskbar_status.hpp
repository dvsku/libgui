#pragma once

namespace libgui {
    enum class taskbar_status : unsigned int {
        no_progress   = 0x0,
        indeterminate = 0x1,
        normal        = 0x2,
        error         = 0x4,
        paused        = 0x8
    };
}
