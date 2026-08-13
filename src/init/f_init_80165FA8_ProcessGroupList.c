#include "../../include/types.h"

extern u8 *D_8011467C;
extern u8 *D_80114688;
extern u8 *D_80168A60;
extern u8 *D_80168A70;
extern u8 *D_80114694;
extern u8 *D_80168A90;

extern u8 func_80165DE8(s32 a0, u8 *a1, void *out);   /* not yet decompiled */
extern s32 func_8002576C(void *ctx);                    /* not yet decompiled */

/* Points three globals at fixed tables, then walks `count` (arg1) entries
   of a 0xC-byte-stride array at arg2: entry.unk0 (s16) is a sub-count,
   entry.unk4 is a pointer array (stride 4) of byte pointers, entry.unk8 is
   the base of a parallel 0x2C-stride output array. For each sub-entry
   whose pointer's first byte is nonzero: resolves it via func_80165DE8 into
   the matching output slot; if that succeeds (low byte set) and
   func_8002576C(slot) also succeeds, the running result is reset to 0
   (mirrors the pattern in f_init_80165F00_ScanEntryTable). Returns the low
   byte of the last-touched result (starts at 1, so an entirely-empty list
   returns 1). */
u8 f_init_80165FA8_ProcessGroupList(s32 arg0, s16 count, void *arg2) {
    u8 *group = (u8 *) arg2;
    u8 *groupEnd = group + count * 0xC;
    u8 result = 1;

    D_8011467C = D_80168A70;
    D_80114688 = D_80168A60;
    D_80114694 = D_80168A90;

    while (group < groupEnd) {
        s16 subCount = *(s16 *) (group + 0x0);
        u8 **ptrs = *(u8 ***) (group + 0x4);
        u8 *out = *(u8 **) (group + 0x8);
        s32 i;

        for (i = 0; i < subCount; i++) {
            u8 *entry = ptrs[i];

            if (*entry != 0) {
                result = func_80165DE8(arg0, entry, out + i * 0x2C);
                if (result != 0 && func_8002576C(out + i * 0x2C) != 0) {
                    result = 0;
                }
            }
        }

        group += 0xC;
    }

    return result;
}
