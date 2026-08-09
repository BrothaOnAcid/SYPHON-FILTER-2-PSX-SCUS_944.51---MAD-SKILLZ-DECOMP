#include "../../include/types.h"

/* Not in asm/800.s: 0x80104DEC falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c. */

extern void *g_main_8011E374_MixerPtr;   /* guess: pointer to a live mixer/output object, see f_main_80104D3C_ApplyVolume */

/* guess: applies a value pair to the current mixer object (halfwords at
   +0x1B0/+0x1B2, near the +0x184/+0x186 volume pair) - likely a pan or
   reverb parameter given the call site in f_main_800FBAE4_SetChannelParam. */
void f_main_80104DEC_ApplyPan(s16 l, s16 r) {
    *(s16 *) ((u8 *) g_main_8011E374_MixerPtr + 0x1B0) = l;
    *(s16 *) ((u8 *) g_main_8011E374_MixerPtr + 0x1B2) = r;
}
