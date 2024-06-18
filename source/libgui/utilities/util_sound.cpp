#include "libgui/utilities/util_sound.hpp"

#include <windows.h>
#include <playsoundapi.h>

void libgui::sound::warning() {
    PlaySound((LPCTSTR)SND_ALIAS_SYSTEMHAND, NULL, SND_ALIAS_ID | SND_SYSTEM | SND_ASYNC);
}

void libgui::sound::success() {
    PlaySound((LPCTSTR)SND_ALIAS_SYSTEMASTERISK, NULL, SND_ALIAS_ID | SND_SYSTEM | SND_ASYNC);
}
