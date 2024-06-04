#include "dv_gui_opengl/dv_gui_opengl/utilities/dv_util_sound.hpp"

#include <windows.h>
#include <playsoundapi.h>

using namespace dvsku;

void dv_util_sound::warning() {
    PlaySound((LPCTSTR)SND_ALIAS_SYSTEMHAND, NULL, SND_ALIAS_ID | SND_SYSTEM | SND_ASYNC);
}

void dv_util_sound::success() {
    PlaySound((LPCTSTR)SND_ALIAS_SYSTEMASTERISK, NULL, SND_ALIAS_ID | SND_SYSTEM | SND_ASYNC);
}
