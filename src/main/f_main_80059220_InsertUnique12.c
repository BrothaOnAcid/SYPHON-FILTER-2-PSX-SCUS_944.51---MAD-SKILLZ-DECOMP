#include "../../include/types.h"

extern void **g_main_8011EEFC_ObjArray;

/* guess: inserts ObjArray[id] into a fixed 12-slot table (ctx+0x2E4,
   stride 0x80, value at slot+4) if it isn't already present, using the
   first free (zero) slot. No-op if the value is already present or the
   table is full. */
void f_main_80059220_InsertUnique12(void *ctx, s32 id) {
    u8 *base = (u8 *) ctx + 0x2E4;
    s32 target = (s32) PTR_U32(g_main_8011EEFC_ObjArray[id]);
    s32 slot = -1;
    s32 i;

    for (i = 0; i < 12; i++) {
        s32 *p = (s32 *) (base + 4 + i * 0x80);

        if (*p == 0) {
            if (slot < 0)
                slot = i;
        } else if (*p == target) {
            slot = i;
        }
    }

    if (slot >= 0)
        *(s32 *) (base + 4 + slot * 0x80) = target;
}
