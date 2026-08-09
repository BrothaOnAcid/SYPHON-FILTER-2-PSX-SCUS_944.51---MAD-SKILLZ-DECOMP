#include "../../include/types.h"

/* Not in asm/800.s: 0x800FBF64 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c.

   Pure fixed-point math, no engine/hardware calls - transcribed the
   multiply/shift constant-division idioms bit-for-bit (mult+mfhi+shift is
   the standard signed-divide-by-constant compiler pattern) rather than
   guessing the actual divisor, to guarantee numeric equivalence. Used by
   f_main_800FBAE4_SetChannelParam to rescale a raw pan value by the
   channel's current volume (D_8011CFDC[index]) with a squared curve -
   looks like a perceptual volume/pan law. */

extern s32 D_8011CFDC[];   /* per-index last-set parameter value, see f_main_800FBAE4_SetChannelParam */

s32 f_main_800FBF64_ScaleByVolumeCurve(s16 raw, s32 index) {
    s32 a0 = (s32) raw * D_8011CFDC[index];
    s32 v0 = (s32) (((s64) a0 * (s32) 0x81020409) >> 32) + a0;
    s32 sign;

    v0 = v0 >> 6;
    v0 -= a0 >> 31;

    sign = (v0 >= 0) ? 1 : -1;

    {
        s32 sq = v0 * v0;
        s32 v0b = (s32) (((s64) sq * (s32) 0x80020009) >> 32) + sq;

        v0b = v0b >> 13;
        v0b -= sq >> 31;

        return (s16) (v0b * sign);
    }
}
