#include "../../include/types.h"

/* Not in asm/800.s: 0x80107A58 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c.

   A second, unreachable-by-static-jal-scan fragment immediately follows
   this function at 0x80107AFC-0x80107B54 (same pattern as the dead code
   noted near 0x800FA7AC) - not decompiled, likely reached only via a
   function pointer. */

extern s32 g_main_8011E398_unk;   /* guess: "wrap enabled" flag */
extern s32 g_main_8011E39C_unk;   /* guess: output right-shift amount */
extern s32 g_main_8011E3A0_unk;   /* guess: wrap divisor */
extern s32 g_main_8011E3A4_unk;   /* guess: wrap mask */
extern void *g_main_8011E374_MixerPtr;   /* see f_main_80104D3C_ApplyVolume */

/* guess: optionally wraps `value` into [0, divisor) (adding the divisor
   and masking if it's negative-mod), then right-shifts by a global
   amount. `channel` == -2 returns the wrapped-but-unshifted value; == -1
   returns the shifted value (as u16); anything else stores the shifted
   value into the per-channel halfword array at g_main_8011E374_MixerPtr
   and returns an unspecified value (unused by the store path in the
   original asm). */
s32 f_main_80107A58_ScaleAndStore(s32 channel, s32 value) {
    s32 shifted;

    if (g_main_8011E398_unk != 0) {
        u32 divisor = (u32) g_main_8011E3A0_unk;
        u32 rem = (u32) value % divisor;

        if (rem != 0) {
            value = (value + (s32) divisor) & ~g_main_8011E3A4_unk;
        }
    }

    shifted = (u32) value >> g_main_8011E39C_unk;

    if (channel == -2) {
        return value;
    }
    if (channel == -1) {
        return (u16) shifted;
    }

    *((u16 *) g_main_8011E374_MixerPtr + channel) = (u16) shifted;
    return value;
}
