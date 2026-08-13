#include "../../include/types.h"

extern SoundSrcEntry *f_main_8001926C_FindNearestSoundSource(SoundListener *self, PosSource *target);

/* Returns whether a sound source was found near target and has a valid core. */
s32 f_main_800192B8_HasSoundSourceNear(SoundListener *self, PosSource *target) {
    SoundSrcEntry *hit = f_main_8001926C_FindNearestSoundSource(self, target);

    if (hit == 0)
        return 0;
    return hit->core != 0;
}
