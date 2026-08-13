#include "../../include/types.h"

/* Reads one word from the per-slot table entry at `self + self->unk134*0x3C`
   (same base as f_main_80031838_GetSlotState's `self + i*0x3C + 0xE60`
   table): mode 0 -> +0xE68, mode 1 -> +0xE6C, anything else -> 0. */
s32 f_main_80031730_GetSlotField(void *self, s32 mode) {
    u32 i;

    if (mode == 0) {
        i = ((u32 *) self)[0x4D]; /* +0x134: slot index */
        return *(s32 *) ((u8 *) self + i * 0x3C + 0xE68);
    } else if (mode == 1) {
        i = ((u32 *) self)[0x4D]; /* +0x134: slot index */
        return *(s32 *) ((u8 *) self + i * 0x3C + 0xE6C);
    }
    return 0;
}
