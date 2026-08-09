#include "../../include/types.h"

/* Walks a +0x18-linked chain of objects (same slot layout as
   f_main_800A730C_ResetCurrentSlots: +0x0 base, +0xC u16 count, stride
   0x10 elements), adding dx/dy to each element's +0x8/+0xA u16 fields. */
void f_main_800A58AC_OffsetSlotsChain(void *arg0, s32 dx, s32 dy) {
    void *node;
    u16 count;
    s32 i;
    u8 *base;

    for (node = arg0; node != 0; node = *(void **) ((u8 *) node + 0x18)) {
        count = *(u16 *) ((u8 *) node + 0xC);
        if ((s16) count <= 0)
            continue;

        base = *(u8 **) ((u8 *) node + 0x0);
        for (i = 0; i < (s16) count; i++) {
            u8 *entry = base + i * 0x10;
            *(u16 *) (entry + 0x8) += dx;
            *(u16 *) (entry + 0xA) += dy;
        }
    }
}
