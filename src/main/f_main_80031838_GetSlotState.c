#include "../../include/types.h"

/* Fills `out` with the current state of `self`'s weapon slot whose active
   sub-object is `sub` (one of self's +0xDC/+0xE0 sub-objects, as chosen by
   func_80031790). Called by func_8003191C/func_80035FC0/func_80036008 before
   handing the result to func_8002EFF0 (slot +0x74..0x80) or func_80032C38.

   `out->val` starts 0, is forced to 0x46 when the sub-object's byte +0x26 is
   in {1,2}, and is overridden by a value from self's +0x130C table (skipping
   the 0x80000001 sentinel) when a per-weapon flag bit is set. Exact meanings
   unconfirmed. */
void f_main_80031838_GetSlotState(void *self, void *sub, SlotState *out) {
    u32 i;
    u32 j;
    u32 k;
    u32 v;
    u8 *p;
    u32 pa;

    out->unk00 = 0;
    out->val = 0;
    out->unk08 = 0;

    if (sub != 0) {
        if ((u8) (((u8 *) sub)[0x26] - 1) < 2) { /* guess: sub-object type/mode in {1,2} */
            out->val = 0x46;
        }
    }

    if (sub == ((void **) self)[0x37]) { /* +0xDC: primary sub-object */
        i = ((u32 *) self)[0x4D];              /* +0x134: slot index */
        j = *(u32 *) ((u8 *) self + i * 0x3C + 0xE60); /* guess: per-slot table, stride 0x3C */
        p = (u8 *) self + j * 0xE0 + 0x140;    /* guess: per-weapon block, stride 0xE0 */
        pa = PTR_U32(p);
        /* unaligned flag-bit read: word index (pa&3)*8+4 >> 5 folds to 0 */
        if (*(u32 *) (p - (pa & 3)) & (1u << ((pa & 3) * 8 + 4))) { /* guess: flag bit test */
            k = ((u32 *) self)[0x4C3];              /* +0x130C: index */
            v = *(u32 *) ((u8 *) self + k * 4 + 0x130C);
            if (v != 0x80000001) {                  /* guess: "empty" sentinel */
                out->val = v;
            }
        }
    }
}
