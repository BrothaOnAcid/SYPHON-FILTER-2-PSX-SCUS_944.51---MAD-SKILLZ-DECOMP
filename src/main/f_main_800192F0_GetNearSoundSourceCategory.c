#include "../../include/types.h"

extern SoundSrcEntry *f_main_8001926C_FindNearestSoundSource(SoundListener *self, PosSource *target);

/* Returns the nearest sound source's category/type id (SoundSrcInfo.unk20->unk2),
   or -1 if none was found / it has no core. */
s16 f_main_800192F0_GetNearSoundSourceCategory(SoundListener *self, PosSource *target) {
    SoundSrcEntry *hit = f_main_8001926C_FindNearestSoundSource(self, target);

    if (hit == 0)
        return -1;
    if (hit->core == 0)
        return -1;
    return ((TargetInfo *) hit->info->unk20)->unk2;
}
