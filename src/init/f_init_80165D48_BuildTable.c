#include "../../include/types.h"

extern void func_80100260(u16 *dst, s32 count, s32 scaledValue); /* main module, undecompiled */

extern u16 g_main_80114718_Table[0x100];
extern u16 g_main_80114918_unk;
extern u16 g_main_8011491A_unk;
extern u16 g_main_8011491C_unk;
extern u16 g_main_8011491E_unk;

/* Copies `src`'s header fields into the globals right after
   g_main_80114718_Table, populates the 256-entry u16 table via
   func_80100260, then normalizes every nonzero-low-15-bits entry by
   forcing its sign bit on. */
void f_init_80165D48_BuildTable(TableSrc *src) {
    u16 *entry;
    u16 val;
    s32 i;

    g_main_80114918_unk = src->unk1C;
    g_main_8011491A_unk = src->unk1E;
    g_main_8011491C_unk = src->unk20;
    g_main_8011491E_unk = src->unk22;

    func_80100260(g_main_80114718_Table, src->unk24, (s32) (src->unk20 << 16) >> 15);

    for (i = 0, entry = g_main_80114718_Table; i < 0x100; i++, entry++) {
        val = *entry;
        if (val & 0x7FFF)
            *entry = val | 0x8000;
    }
}
