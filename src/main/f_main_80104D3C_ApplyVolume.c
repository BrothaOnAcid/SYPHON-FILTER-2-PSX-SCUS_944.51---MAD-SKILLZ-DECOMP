#include "../../include/types.h"

/* Not in asm/800.s: 0x80104D3C falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c. Reads/writes
   fixed absolute addresses in main's own data area (0x8011Exxx), not
   gp-relative - same cross-module-mirror pattern noted in knowledge.txt
   for f_main_800FD1CC_ActivateChannels. */

extern void *g_main_8011E374_MixerPtr;   /* guess: pointer to a live mixer/output object */
extern s16 g_main_8011E3FC_VolL;         /* guess: main-side volume mirror, low half */
extern s16 g_main_8011E3FE_VolR;

/* guess: applies the master volume to the current mixer object (halfwords
   at +0x184/+0x186) and mirrors it into a fixed main-side pair. */
void f_main_80104D3C_ApplyVolume(s16 volL, s16 volR) {
    *(s16 *) ((u8 *) g_main_8011E374_MixerPtr + 0x184) = volL;
    *(s16 *) ((u8 *) g_main_8011E374_MixerPtr + 0x186) = volR;

    g_main_8011E3FC_VolL = volL;
    g_main_8011E3FE_VolR = volR;
}
