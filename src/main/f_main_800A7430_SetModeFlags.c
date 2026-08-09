#include "../../include/types.h"

extern void *func_800A5718(void);
extern void func_800A5758(void *obj);

/* mode: 0 -> set bit 0x8 (clear 0x40), 1 -> set bit 0x40 (clear 0x8), else
   clear both 0x8 and 0x40 - all on the +0x14 flags byte of the "current"
   object returned by func_800A5718 (bit 0x48 always cleared as a side
   effect of the mask). No-op if there's no current object. */
void f_main_800A7430_SetModeFlags(s32 mode) {
    void *obj = func_800A5718();
    u8 flags;

    if (obj == 0)
        return;

    flags = *((u8 *) obj + 0x14);
    if (mode == 0) {
        flags = (flags | 0x8) & 0xBF;
    } else if (mode == 1) {
        flags = (flags | 0x40) & 0xF7;
    } else {
        flags = flags & 0xB7;
    }

    *((u8 *) obj + 0x14) = flags;
    func_800A5758(obj);
}
