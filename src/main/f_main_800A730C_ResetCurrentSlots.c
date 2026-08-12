#include "../../include/types.h"

extern void *f_main_800A5718_LookupHandle(void);

/* Resets the "current" object's entry count (+0x4) to 0 and poisons the
   +0x4/+0x5/+0x6 bytes of each of its +0xC (u16 count) elements (stride
   0x10, base at +0x0) to 0xFF. No-op if there's no current object. */
void f_main_800A730C_ResetCurrentSlots(void) {
    void *obj = f_main_800A5718_LookupHandle();
    u16 count;
    s32 i;
    u8 *base;

    if (obj == 0)
        return;

    count = *(u16 *) ((u8 *) obj + 0xC);
    if ((s16) count <= 0)
        return;

    *(s32 *) ((u8 *) obj + 0x4) = 0;
    base = *(u8 **) ((u8 *) obj + 0x0);

    for (i = 0; i < (s16) count; i++) {
        u8 *entry = base + i * 0x10;
        entry[0x4] = 0xFF;
        entry[0x5] = 0xFF;
        entry[0x6] = 0xFF;
    }
}
