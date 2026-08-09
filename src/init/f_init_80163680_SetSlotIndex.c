#include "../../include/types.h"

extern s32 func_8008F844(void *self, ResourceSub **out); /* main module, undecompiled */

/* Resolves `self`'s ResourceSub (via func_8008F844's self->+0xC->+0x160
   chain) and stores `slotIndex` into its +0x134 field. Returns 0 only
   when func_8008F844 itself reports failure (self or self->+0xC null). */
s32 f_init_80163680_SetSlotIndex(void *self, s32 slotIndex) {
    ResourceSub *sub;
    s32 ok;

    ok = func_8008F844(self, &sub) & 0xFF;
    if (sub == 0 || ok == 0)
        return 0;

    sub->unk134 = slotIndex;
    return 1;
}
