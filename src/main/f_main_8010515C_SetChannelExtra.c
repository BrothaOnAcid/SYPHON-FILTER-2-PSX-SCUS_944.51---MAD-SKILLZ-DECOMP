#include "../../include/types.h"

/* Not in asm/800.s: 0x8010515C falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c. */

extern void *g_main_8011E374_MixerPtr;   /* see f_main_80104D3C_ApplyVolume */

/* guess: writes a 15-bit-masked value pair into a per-channel 16-byte slot
   (channel*0x10) at the base mixer object. Followed by a small fixed
   2-iteration loop (acc = 1; acc *= 13 twice) whose result is never
   stored or returned - looks like dead/vestigial code (maybe a leftover
   compiler artifact or an intentional busy-wait), transcribed as-is. */
void f_main_8010515C_SetChannelExtra(s32 channel, s16 a1, s16 a2) {
    u8 *slot = (u8 *) g_main_8011E374_MixerPtr + channel * 0x10;
    s32 acc = 1;
    s32 i;

    *(s16 *) (slot + 0) = a1 & 0x7FFF;
    *(s16 *) (slot + 2) = a2 & 0x7FFF;

    for (i = 0; i < 2; i++) {
        acc = acc * 13;
    }
}
