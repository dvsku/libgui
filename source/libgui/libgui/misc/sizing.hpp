#pragma once

#include <cstdint>

namespace libgui {
    template<typename T>
    struct point {
        T x = 0;
        T y = 0;
    };

    using point_i = point<int32_t>;
    using point_f = point<float>;

    template<typename T>
    struct size {
        T width  = 0;
        T height = 0;

        size<T>& scale(float factor) {
            width  = static_cast<T>(width  * factor);
            height = static_cast<T>(height * factor);

            return *this;
        }
    };

    using size_i = size<int32_t>;
    using size_f = size<float>;

    template<typename T>
    struct rect {
        T left   = 0;
        T top    = 0;
        T right  = 0;
        T bottom = 0;

        rect<T>& scale(float factor) {
            left   = static_cast<T>(left   * factor);
            top    = static_cast<T>(top    * factor);
            right  = static_cast<T>(right  * factor);
            bottom = static_cast<T>(bottom * factor);

            return *this;
        }
    };

    using rect_i = rect<int32_t>;
    using rect_f = rect<float>;
}
