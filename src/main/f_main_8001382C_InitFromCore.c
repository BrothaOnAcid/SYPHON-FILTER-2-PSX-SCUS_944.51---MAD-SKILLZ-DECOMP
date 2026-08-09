#include "../../include/types.h"

/* --- not yet decompiled --- */
extern void func_800F042C(u16 arg0);
extern void func_800F4200(void *dst);
extern void func_8001084C(void *tag);

/* Snapshots `self->core`'s first 8 words into self+0x1C..+0x3C, zeroes
   self->unk18, re-derives self->unk68 from self->core (func_800F042C/
   func_800F4200), then temporarily mirrors 4 s16 fields + 1 s32 field of
   `target` (negate, call func_8001084C(target), negate back) - likely
   flipping a position/rotation record into a mirrored space for that one
   call. Companion of f_main_8001C828_ProcessSoundSources (same
   func_8001084C tag-context call). */
void f_main_8001382C_InitFromCore(SoundListener *self, MirrorObj *target) {
    ListenerCore *core = self->core;
    s32 i;

    self->unk18 = 0;
    for (i = 0; i < 8; i++) {
        self->snapshot[i] = ((s32 *) core)[i];
    }

    func_800F042C(self->unk4);
    func_800F4200(self->unk68);

    target->unk2 = -target->unk2;
    target->unkA = -target->unkA;
    target->unk6 = -target->unk6;
    target->unk18 = -target->unk18;
    target->unkE = -target->unkE;

    func_8001084C(target);

    target->unk2 = -target->unk2;
    target->unkA = -target->unkA;
    target->unk6 = -target->unk6;
    target->unk18 = -target->unk18;
    target->unkE = -target->unkE;
}
