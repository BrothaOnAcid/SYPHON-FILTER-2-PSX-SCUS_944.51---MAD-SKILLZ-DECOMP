#include "../../include/types.h"

extern s32 g_main_8011F9D0_unk;
extern s32 g_main_8013BB10_unk[]; /* guess: 6 words, stride 0x3C (in dwords: 0xF) */
extern s32 g_main_8013BB40_unk[]; /* guess: 6 words, stride 0x3C */

/* Zeroes one word from each of two 6-entry tables at matching offsets
   (stride 0x3C), plus a standalone flag. The asm has a nested loop shape
   but the inner loop's bound is always 1, so it only ever runs once per
   outer step - kept as a single pass here. */
void f_init_80167F64_ClearSlots(void) {
    s32 i;

    g_main_8011F9D0_unk = 0;

    for (i = 0; i < 6; i++) {
        s32 off = i * 0x3C;
        *(s32 *) ((u8 *) g_main_8013BB10_unk + off) = 0;
        *(s32 *) ((u8 *) g_main_8013BB40_unk + off) = 0;
    }
}
