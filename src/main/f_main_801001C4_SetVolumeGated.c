#include "../../include/types.h"

/* Not in asm/800.s: 0x801001C4 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. Immediately follows
   f_main_8010008C_SetAudioMode in the raw dump. This whole blocked range
   (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not main's
   0x8011EC64 - see f_main_800FC814_DestroyMmid.c. */

extern AudioSeqState g_main_8012F41C_AudioSeq;

extern void f_main_80104D3C_ApplyVolume(s16 volL, s16 volR);

/* guess: records the requested volume, and applies it immediately unless a
   mode switch is currently suppressing volume changes
   (g_main_8012F41C_AudioSeq.volFlag). */
void f_main_801001C4_SetVolumeGated(s16 volL, s16 volR) {
    g_main_8012F41C_AudioSeq.volL = volL;
    g_main_8012F41C_AudioSeq.volR = volR;

    if (g_main_8012F41C_AudioSeq.volFlag == 0) {
        f_main_80104D3C_ApplyVolume(volL, volR);
    }
}
