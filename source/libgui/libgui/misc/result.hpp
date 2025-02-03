#pragma once

#include <string>

namespace libgui {
    struct result {
        bool        success = false;
        std::string message = "";

        explicit operator bool() {
            return success == true;
        }
    };
}
