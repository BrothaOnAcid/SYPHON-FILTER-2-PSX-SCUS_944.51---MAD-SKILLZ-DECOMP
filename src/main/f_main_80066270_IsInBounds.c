#include "../../include/types.h"

/* guess: gated on the actor's core flags (either flags104 bit 0x400 with
   flags154 bit 0x40000, or flags104 bit 0x200 clear); when gated in,
   returns 1 only if unk20/24/28 are all within [-0x1800, 0x1800]
   (unsigned-biased range check), else 0. Ungated, returns 0. */
s32 f_main_80066270_IsInBounds(ActorNode *node) {
    ActorCore *core = node->core;

    if (!((core->flags104 & 0x400) && (core->flags154 & 0x40000)) && (core->flags104 & 0x200))
        return 0;

    if ((u32) (core->unk20 + 0x1800) >= 0x3001)
        return 0;
    if ((u32) (core->unk24 + 0x1800) >= 0x3001)
        return 0;
    if ((u32) (core->unk28 + 0x1800) >= 0x3001)
        return 0;

    return 1;
}
