#include "../../include/types.h"

/* Not in asm/800.s: 0x800FBBDC falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c. */

extern s16 g_main_8012F3F0_unk;

extern s32 f_main_800F616C_SendEngineConfig(u8 *buf4);

/* guess: records `flag` and builds a 4-byte config buffer for
   f_main_800F616C_SendEngineConfig - {0x7F,0,0x7F,0} when `flag` is 0, or
   all 0x7F otherwise. */
void f_main_800FBBDC_ApplyConfigFlag(s32 flag) {
    u8 buf[4];

    g_main_8012F3F0_unk = (s16) flag;

    if (flag == 0) {
        buf[0] = 0x7F;
        buf[1] = 0;
        buf[2] = 0x7F;
        buf[3] = 0;
    } else {
        buf[0] = 0x7F;
        buf[1] = 0x7F;
        buf[2] = 0x7F;
        buf[3] = 0x7F;
    }

    f_main_800F616C_SendEngineConfig(buf);
}
