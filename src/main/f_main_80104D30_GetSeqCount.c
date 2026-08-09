#include "../../include/types.h"

/* Not in asm/800.s: 0x80104D30 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c. */

extern AudioSeqState g_main_8012F41C_AudioSeq;

s32 f_main_80104D30_GetSeqCount(void) {
    return g_main_8012F41C_AudioSeq.count;
}
