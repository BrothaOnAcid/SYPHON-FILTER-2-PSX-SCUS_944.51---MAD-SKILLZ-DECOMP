#include "../../include/types.h"

extern SoundSrcEntry *func_80016CAC(SoundListener *self, void *outPos);

/* Builds a query position from target's core (x, -y, z) and looks up the
   nearest active sound source near it via func_80016CAC. */
SoundSrcEntry *f_main_8001926C_FindNearestSoundSource(SoundListener *self, PosSource *target) {
    ListenerCore *core = target->core;
    s32 pos[3];

    pos[0] = core->posX;
    pos[1] = -core->posY;
    pos[2] = core->posZ;

    return func_80016CAC(self, pos);
}
