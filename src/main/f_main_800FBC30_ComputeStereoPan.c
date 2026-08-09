#include "../../include/types.h"

/* Not in asm/800.s: 0x800FBC30 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c.

   Very dense fixed-point trig/geometry code (820 bytes) - no engine or
   hardware calls anywhere in it, just arithmetic against a table at
   D_8011E044 (s16 pairs, presumably a sin/cos-ish table) and the existing
   PlaySongNode/SetChannelParam callers, so kept in scope (not a "psyq
   call"). Transcribed mechanically, preserving the exact multiply/shift
   constant-division idioms (same two magic constants as
   f_main_800FBF64_ScaleByVolumeCurve - factored into divK1/divK2 helpers
   here) rather than inventing semantic names for every intermediate;
   confidence on the *meaning* of most of this is low, confidence on the
   *arithmetic* being bit-exact is high. `buf` is the 8th (stack) arg, not
   the 1st - both existing callers' externs had this wrong, fixed here.

   CAVEAT: the sign-negation logic below (using the prior contents of buf
   as a hint, then a final sign correction via dividing a prior half by
   its own absolute value) has deeply nested bgez/bltz branches in the
   real asm that weren't fully untangled bit-for-bit - this is a
   reasonable-effort approximation of the same net behavior, not a
   guaranteed-exact transcription of that specific sub-block. Revisit
   before trusting this in a real build. */

extern s16 g_main_8012F3F0_unk;   /* see f_main_800FBBDC_ApplyConfigFlag */
extern s16 D_8011E044[];          /* guess: sin/cos-ish table, s16 pairs */

static s32 divK1(s32 x) {
    s32 v0 = (s32) (((s64) x * (s32) 0x81020409) >> 32) + x;

    v0 = v0 >> 6;
    v0 -= x >> 31;
    return v0;
}

static s32 divK2(s32 x) {
    s32 v0 = (s32) (((s64) x * (s32) 0x80020009) >> 32) + x;

    v0 = v0 >> 13;
    v0 -= x >> 31;
    return v0;
}

/* guess: computes a stereo pan pair for `buf` from a combined
   volume/priority chain (a0/a1/a3/f) and an angle (a2, adjusted by e/g,
   folded into a table index), consulting the prior contents of `buf` for
   sign hints. If g_main_8012F3F0_unk == 1, skips the angle math entirely
   and writes the same mono value to both halves. Writes 0/0 if the
   combined volume chain reduces to 0. */
void f_main_800FBC30_ComputeStereoPan(s32 a0, s32 a1, s32 a2, s32 a3, s32 e, s32 f, s32 g, s16 *buf) {
    s32 t0 = divK1((a1 * 129) * f);

    t0 = divK1(t0 * a3);
    t0 = divK1(t0 * a0);

    if (t0 == 0) {
        buf[0] = 0;
        buf[1] = 0;
        return;
    }

    if (g_main_8012F3F0_unk == 1) {
        buf[0] = (s16) t0;
        buf[1] = (s16) t0;
        return;
    }

    a2 = a2 + (g + e);

    if (a2 >= 0x168) {
        a2 -= 0x168;
    }
    if (a2 < 0x10E) {
        a2 += 0x5A;
    } else {
        a2 -= 0x10E;
    }

    {
        s32 panL, panR;
        s16 priorL = buf[0];
        s16 priorR = buf[1];
        s32 absL, absR, sign;

        if (a2 < 0xB4) {
            panL = (s32) D_8011E044[a2 * 2 + 0];
            panR = (s32) D_8011E044[a2 * 2 + 1];
        } else {
            a2 -= 0xB4;
            panL = (s32) D_8011E044[a2 * 2 + 0];
            panR = (s32) D_8011E044[a2 * 2 + 1];
        }

        panL = t0 * panL;
        panR = t0 * panR;
        panL = divK2(panL);
        panR = divK2(panR);

        if (priorL < 0) {
            panL = -panL;
            panR = -panR;
        }

        absL = (priorL < 0) ? -priorL : priorL;
        absR = (priorR < 0) ? -priorR : priorR;

        if (absR < absL) {
            sign = (absL != 0) ? priorL / absL : 0;
        } else {
            sign = (absR != 0) ? priorR / absR : 0;
        }

        buf[0] = (s16) (panL * sign);
        buf[1] = (s16) (panR * sign);
    }
}
