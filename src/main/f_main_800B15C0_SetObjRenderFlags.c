#include "../../include/types.h"

extern void **g_main_8011EEFC_ObjArray;

/* guess: sets GPU-primitive-style flag bits on ObjArray[id]'s sub-block
   (+0x1C), and stashes `flag` into a byte field (+0x15). Bit layout
   (0x10800000 set / 0xD7FCFFFF mask, +0x100 gating a +0x4 bit) looks like
   packed OT/semi-transparency flags, not confirmed further. */
void f_main_800B15C0_SetObjRenderFlags(s32 id, s8 flag) {
    void *obj = g_main_8011EEFC_ObjArray[(s16) id];
    u32 *block = *(void **) ((u8 *) obj + 0x1C);

    block[0] |= 0x10800000;
    block[0] &= 0xD7FCFFFF;
    if (block[0] & 0x100)
        block[1] |= 0x2000;

    *((u8 *) block + 0x15) = (u8) flag;
}
