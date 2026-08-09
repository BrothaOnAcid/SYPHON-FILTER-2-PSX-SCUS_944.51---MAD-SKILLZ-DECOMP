#include "../../include/types.h"

/* Not in asm/800.s: 0x8010595C falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c. */

extern void *g_main_8011E374_MixerPtr;   /* see f_main_80104D3C_ApplyVolume */

/* guess: applies a 15-bit-masked value pair to the current mixer object
   (halfwords at +0x180/+0x182, just before the +0x184/+0x186 volume pair);
   called with a scaled value from f_main_800FBAE4_SetChannelParam's index
   3 case, so likely a pitch/tune parameter. */
void f_main_8010595C_ApplyPitch(s16 a0, s16 a1) {
    *(s16 *) ((u8 *) g_main_8011E374_MixerPtr + 0x180) = a0 & 0x7FFF;
    *(s16 *) ((u8 *) g_main_8011E374_MixerPtr + 0x182) = a1 & 0x7FFF;
}
