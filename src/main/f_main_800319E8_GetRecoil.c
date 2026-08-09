#include "../../include/types.h"

/* Returns -0x20 (a penalty/bonus delta, exact meaning unconfirmed) when all of
   the following hold, else 0:
     - the primary sub-object self->+0xDC exists and its +0x10 pointer's byte
       +0xC is 8 (a weapon "type")
     - a per-weapon flag bit is set, read via an unaligned-word trick at that
       +0x10 object (bit offset (addr&3)*8+0x15)
     - the slot index self->+0x134 is in {8,9,10} (bit test on constant 0x700)
   Callers accumulate the result into a weapon slot attribute value
   (func_80031A78 into the per-weapon block at +0x8, func_80035A48 into the
   slot-array entry at +0x4). `unused` is received but never read (it only
   appears in dead delay-slot `andi` instructions in the asm). */
s32 f_main_800319E8_GetRecoil(void *self, u32 unused) {
    u32 *w;
    u32 slot;
    u32 bit;

    w = ((void **) ((void **) self)[0x37])[4]; /* +0xDC->+0x10 */
    if (((u8 *) w)[0xC] != 8)
        return 0;

    slot = ((u32 *) self)[0x4D]; /* +0x134: slot index */

    bit = (PTR_U32(w) & 3) * 8 + 0x15;
    if (!(((u32 *) (w - (PTR_U32(w) & 3)))[bit >> 5] & (1u << (bit & 31))))
        return 0;

    if (!((0x700 >> (slot & 31)) & 1)) /* guess: slot in {8,9,10} */
        return 0;

    return -0x20;
}
