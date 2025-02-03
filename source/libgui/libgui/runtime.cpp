#include "libgui/runtime.hpp"
#include "libgui/internals/global.hpp"

using namespace libgui;

void libgui::set_backend(int8_t type) {
    internals::global::backend_type = type;
}
