#include "../../include/types.h"

extern void f_main_80025E40_MemsetWords(void *dst, u8 val, u32 size);
extern void func_800F4190(void *dst, s32 value, s32 size); /* memset, see DOC.md (blocked PSYQ library region) */

extern u8 g_main_801283AC_unk[];   /* guess: buffer, 0x230 bytes, zeroed here */
extern u8 g_main_801285DC_unk[];   /* guess: buffer, 0x1950 bytes, zeroed here */
extern s16 g_main_80168AA0_unk;
extern s32 g_main_8011F5CC_unk;
extern s32 g_main_8011F5DC_unk;
extern u8 g_main_8011F5D0_unk[];   /* guess: 8-byte buffer, zeroed here */
extern s32 g_main_8011F5D8_unk;

/* State reset run on INIT.OVL entry: zeroes several main-module buffers/
   flags before the rest of init runs. */
void f_init_8016749C_Reset(void) {
    g_main_80168AA0_unk = 0;
    g_main_8011F5CC_unk = 0;
    g_main_8011F5DC_unk = 0;
    f_main_80025E40_MemsetWords(g_main_801283AC_unk, 0, 0x230);

    f_main_80025E40_MemsetWords(g_main_801285DC_unk, 0, 0x1950);

    func_800F4190(g_main_8011F5D0_unk, 0, 8);
    g_main_8011F5D8_unk = 0;
}
