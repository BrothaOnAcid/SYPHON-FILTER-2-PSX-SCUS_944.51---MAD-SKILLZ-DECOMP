#include "../../include/types.h"

/* Guessed sub-object hung off `self`'s +0x10 (its "core"). */
typedef struct {
    u8 _pad00[0x20];
    void *owner;    /* +0x20 */
    u8 _pad24[0x4];
    u32 flags28;    /* +0x28 */
    s32 unk2C;      /* +0x2C */
} InitCore;

/* Guessed owner object; +0x10 is the "core" sub-object above, +0x18/+0x20/
   +0x24 are cleared here. */
typedef struct {
    u8 _pad00[0x10];
    InitCore *core; /* +0x10 */
    u8 _pad14[0x4];
    s32 unk18;      /* +0x18 */
    u8 _pad1C[0x4];
    s32 unk20;      /* +0x20 */
    s32 unk24;      /* +0x24 */
} InitOwner;

/* Guessed child object; only +0x10 is confirmed. */
typedef struct {
    u8 _pad00[0x10];
    s32 unk10;      /* +0x10 */
} InitChild;

/* --- libc-style externs (not yet decompiled) --- */
extern void func_80166728(InitChild *child, InitOwner *self);

/* Attaches `child` to self->core, sets core's flags28 bit 0x400000, resets
   self->0x18/0x24, then links child in via func_80166728 unless core's
   flags28 bit 0x800000 is already set or (`skipLink` != 0 and
   child->0x10 != 0). Finally stores `tag` into core->0x2C and clears
   self->0x20. */
void f_init_801669CC_AttachChild(InitOwner *self, InitChild *child, s32 tag, s32 skipLink) {
    InitCore *core = self->core;

    core->owner = child;
    core->flags28 |= 0x400000;

    self->unk18 = 0;
    self->unk24 = 0;

    if ((core->flags28 & 0x800000) == 0) {
        if (skipLink == 0 || child->unk10 == 0)
            func_80166728(child, self);
    }

    self->core->unk2C = tag;
    self->unk20 = 0;
}
