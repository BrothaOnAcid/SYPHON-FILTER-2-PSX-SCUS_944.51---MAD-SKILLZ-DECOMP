#include "../../include/types.h"

extern s32 D_80114710;                        /* not yet decompiled: a "pending timer/handle" slot, -1 = none */
extern s32 D_8011F9C4;                        /* not yet decompiled: fixed context/handle passed to func_800128D8 */
extern u8 D_80168AE8[];                       /* not yet decompiled: table passed to func_800624CC */
extern u8 D_80168ED8[];                       /* not yet decompiled: table passed to func_800624CC */
extern u8 D_801692C8[];                       /* not yet decompiled: table passed to func_800626AC */
extern s32 D_801696E8;                        /* not yet decompiled: single slot, released via func_800128D8 */
extern s32 D_80169718;                        /* not yet decompiled: 5-entry table, stride 0x18, first word = live flag */

extern void func_800624CC(void *a0, s32 a1, void *a2, void *a3); /* not yet decompiled */
extern void func_800626AC(void *a0, s32 a1, s32 a2, void *a3);   /* not yet decompiled */
extern void func_800A5DCC(s32 handle);                            /* not yet decompiled */
extern void func_800128D8(s32 ctx, s32 *slot);                     /* not yet decompiled: releases *slot under ctx, clears it */

/* Called only from main (near 0x80060318), passing (mode byte, pointer read
   from the caller's own +0x44). Registers `ptr` against two fixed tables
   via func_800624CC/func_800626AC, then - only when `mode` is 0 - runs a
   cleanup pass: releases the pending D_80114710 handle (if any) via
   func_800A5DCC, then releases D_801696E8 and each live entry of the
   5-slot D_80169718 table via func_800128D8. Always returns 1. */
s32 f_init_8015E274_RegisterAndCleanup(u8 mode, void *ptr) {
    s32 i;

    func_800624CC(ptr, 0x15, D_80168AE8, D_80168ED8);
    func_800626AC(ptr, 0x1B, 0x11, D_801692C8);

    if (mode == 0) {
        if (D_80114710 != -1) {
            func_800A5DCC(D_80114710);
            D_80114710 = -1;
        }
        if (D_801696E8 != 0) {
            func_800128D8(D_8011F9C4, &D_801696E8);
        }
        for (i = 0; i < 5; i++) {
            s32 *entry = (s32 *) ((u8 *) &D_80169718 + i * 0x18);
            if (*entry != 0) {
                func_800128D8(D_8011F9C4, entry);
            }
        }
    }

    return 1;
}
