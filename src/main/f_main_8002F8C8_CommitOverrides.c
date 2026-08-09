#include "../../include/types.h"

/* arg0: object with a "base" triple at +0x3C/+0x40/+0x44 and a "current"
   triple at +0x4C/+0x50/+0x54. If arg0->unk4 == 0, unconditionally resets
   the first current slot from base. Otherwise, arg1 (if non-NULL) is a
   per-slot enable mask ({+0x0,+0x4,+0x8} == 1) selecting which of the
   three current slots get reset from base. Returns 0 only when unk4 != 0
   and arg1 is NULL (nothing to commit). */
s32 f_main_8002F8C8_CommitOverrides(void *arg0, void *arg1) {
    u8 *p = (u8 *) arg0;
    s32 *mask = (s32 *) arg1;
    s32 result = 1;

    if (*(s32 *) (p + 0x4) == 0) {
        *(s32 *) (p + 0x4C) = *(s32 *) (p + 0x3C);
    } else if (mask != 0) {
        if (mask[0] == 1)
            *(s32 *) (p + 0x4C) = *(s32 *) (p + 0x3C);
        if (mask[1] == 1)
            *(s32 *) (p + 0x50) = *(s32 *) (p + 0x40);
        if (mask[2] == 1)
            *(s32 *) (p + 0x54) = *(s32 *) (p + 0x44);
    } else {
        result = 0;
    }

    return result;
}
