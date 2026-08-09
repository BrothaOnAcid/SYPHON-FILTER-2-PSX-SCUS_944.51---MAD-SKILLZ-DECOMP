#include "../../include/types.h"

/* Not in asm/800.s: 0x800FD4C0 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c. */

extern Rec48 g_main_801412C0_Rec48[];   /* 24 channel records, 0x48 bytes each */
extern AudioSeqState g_main_8012F41C_AudioSeq;

/* guess: for every set bit i in `mask`, if channel i is active (Rec48[i]
   .unk00 == 1) fires its callback (same (i, unk40, 3) shape as
   f_main_800FD38C_TickTimers's timer-expiry call) and clears bit i from
   g_main_8012F41C_AudioSeq.mask if set there. Finally OR's the whole
   `mask` into AudioSeq.pending. */
void f_main_800FD4C0_NotifyAndClearMask(u32 mask) {
    s32 i;

    for (i = 0; i < 0x18; i++) {
        u32 bit = 1u << i;

        if (mask & bit) {
            if (g_main_801412C0_Rec48[i].unk00 == 1) {
                if (g_main_801412C0_Rec48[i].unk44 != 0) {
                    g_main_801412C0_Rec48[i].unk44(i, g_main_801412C0_Rec48[i].unk40, 3);
                }
            }
            if (g_main_8012F41C_AudioSeq.mask & bit) {
                g_main_8012F41C_AudioSeq.mask &= ~bit;
            }
        }
    }

    g_main_8012F41C_AudioSeq.pending |= mask;
}
