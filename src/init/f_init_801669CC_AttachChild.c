#include "../../include/types.h"

extern void f_init_80166728_LinkNpcSpawnRecords(void *def, void *ctx, s32 n);

/* Attaches `child` to self->core, sets core's flags28 bit 0x400000, resets
   self->0x18/0x24, then links child in via
   f_init_80166728_LinkNpcSpawnRecords (passing `skipLink` through as its
   `n`) unless core's flags28 bit 0x800000 is already set or (`skipLink`
   != 0 and child->0x10 != 0). Finally stores `tag` into core->0x2C and
   clears self->0x20. */
void f_init_801669CC_AttachChild(InitOwner *self, InitChild *child, s32 tag, s32 skipLink) {
    InitCore *core = self->core;

    core->owner = child;
    core->flags28 |= 0x400000;

    self->unk18 = 0;
    self->unk24 = 0;

    if ((core->flags28 & 0x800000) == 0) {
        if (skipLink == 0 || child->unk10 == 0)
            f_init_80166728_LinkNpcSpawnRecords(child, self, skipLink);
    }

    self->core->unk2C = tag;
    self->unk20 = 0;
}
