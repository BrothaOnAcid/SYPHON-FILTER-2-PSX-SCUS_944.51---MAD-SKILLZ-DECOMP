#include "../../include/types.h"

extern s16 g_main_8011ED0E_TypeDefCount;   /* guess: current entry count of the intern table */
extern s16 g_main_8011ED0C_TypeDefCap;     /* guess: valid-entry bound for the linear scan below */
extern s32 *g_main_8011F4B8_TypeDefKeys;   /* guess: parallel array of intern keys, stride 4 */
extern WldTypeDef **g_main_8011F4D4_TypeDefTable; /* guess: parallel array of typeDef pointers, stride 4 */

extern void func_801664D4(void);              /* not yet decompiled: one-time table init */
extern void *f_main_80025AD0_AllocDown(s32 size);
extern void func_80016204(s32 key);            /* not yet decompiled */

/* Finds-or-creates the WldTypeDef interned by `key` (linear scan of
   g_main_8011F4B8_TypeDefKeys, stopping at the first zero/empty slot or at
   g_main_8011ED0C_TypeDefCap). `key == -1` always creates a fresh entry
   (never matches, so placeholder resources never intern together). On
   creation: allocates a 0x78-byte WldTypeDef, appends it + `key` to the
   parallel tables, sets `flags28 = flags` and `unk74 = -1`; if `flags` has
   bit 0x400000 set, the resource is treated as "eager": unless the data at
   `key` starts with the debug/placeholder magic 0xBADBAD00, func_80016204
   (guess: real per-key init) is invoked, else `flags28` gets bit 0x800000
   tacked on instead. In every case (new or reused), `dataPtr`/unk24/unk2C
   are reset to 0 and, if `res` is non-null, `res->typeDef` is set. */
WldTypeDef *f_init_80166568_InternTypeDef(s32 key, WldRes *res, s32 flags) {
    s32 i;
    s32 idx;
    WldTypeDef *typeDef;

    if (g_main_8011ED0E_TypeDefCount == -1) {
        func_801664D4();
    }

    idx = -1;
    if (key != -1 && g_main_8011ED0C_TypeDefCap > 0) {
        for (i = 0; i < g_main_8011ED0C_TypeDefCap; i++) {
            s32 k = g_main_8011F4B8_TypeDefKeys[i];
            if (k == 0) {
                break;
            }
            if (k == key) {
                idx = i;
                break;
            }
        }
    }

    if (idx == -1) {
        idx = g_main_8011ED0E_TypeDefCount;
        typeDef = (WldTypeDef *) f_main_80025AD0_AllocDown(0x78);
        g_main_8011F4D4_TypeDefTable[idx] = typeDef;
        g_main_8011F4B8_TypeDefKeys[idx] = key;
        g_main_8011ED0E_TypeDefCount = idx + 1;
        typeDef->flags28 = flags;
        typeDef->unk74 = -1;

        if (flags & 0x400000) {
            if (*(u32 *) (u64) (u32) key == 0xBADBAD00) {
                typeDef->flags28 |= 0x800000;
            } else {
                func_80016204(key);
            }
        }
    }

    typeDef = g_main_8011F4D4_TypeDefTable[idx];
    typeDef->dataPtr = 0;
    typeDef->unk24 = 0;
    typeDef->unk2C = 0;
    if (res != 0) {
        res->typeDef = typeDef;
    }
    return typeDef;
}
