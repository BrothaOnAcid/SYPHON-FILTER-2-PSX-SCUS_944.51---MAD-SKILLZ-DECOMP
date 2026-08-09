#include "../../include/types.h"

/* Not in asm/800.s: 0x800FBAE4 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c. D_8011CFDC is an
   absolute address (lui/addiu literal in the asm), not gp-relative, so it
   is unaffected by that gp base. */

extern s32 D_8011CFDC[];   /* guess: per-index last-set parameter value */

extern s32 g_main_8012F3F4_unk;  /* guess: "currently active" index */
extern s32 g_main_8012F3F8_unk;
extern s32 g_main_8012F3FC_unk;

extern void f_main_800FBFF0_ReapplyPan(s32 value, s32 index);
extern void f_main_800FBC30_ComputeStereoPan(s32 a0, s32 a1, s32 a2, s32 a3, s32 e, s32 f, s32 g, s16 *buf);
extern s32 f_main_800FBF64_ScaleByVolumeCurve(s16 raw, s32 index);
extern void f_main_80104DEC_ApplyPan(s16 volL, s16 volR);
extern void f_main_8010595C_ApplyPitch(s16 a0, s16 a1);

/* guess: sets per-index channel parameter `value` (clamped to [0,127]).
   `index` == 3 is a special case that scales the (pre-clamp-store) value
   by 129 and forwards it straight to f_main_8010595C_ApplyPitch. Otherwise the value is
   recorded in D_8011CFDC[index] and forwarded via f_main_800FBFF0_ReapplyPan; if
   `index` is also the "currently active" one (g_main_8012F3F4_unk), a
   left/right pair is recomputed via f_main_800FBC30_ComputeStereoPan/f_main_800FBF64_ScaleByVolumeCurve and
   applied through f_main_80104DEC_ApplyPan. Exact numeric semantics beyond the
   control flow are not well understood - transcribed for accuracy from
   raw asm, not verified against known PSYQ SPU library behavior. */
void f_main_800FBAE4_SetChannelParam(s32 index, s32 value) {
    s16 buf[2];   /* sp+0x20/0x22, filled by f_main_800FBC30_ComputeStereoPan */
    s32 r1, r2;

    if (value >= 0x80) {
        value = 0x7F;
    }
    if (value < 0) {
        value = 0;
    }

    if (index == 3) {
        s16 scaled = (s16) (value * 129);
        f_main_8010595C_ApplyPitch(scaled, scaled);
        return;
    }

    D_8011CFDC[index] = value;
    f_main_800FBFF0_ReapplyPan(value, index);

    if (index == g_main_8012F3F4_unk) {
        f_main_800FBC30_ComputeStereoPan(0x7F, g_main_8012F3F8_unk, g_main_8012F3FC_unk, 0x7F, 0, 0x7F, 0, buf);

        r1 = f_main_800FBF64_ScaleByVolumeCurve(buf[0], index);
        buf[0] = (s16) ((s16) r1 * 2);

        r2 = f_main_800FBF64_ScaleByVolumeCurve(buf[1], index);
        buf[1] = (s16) ((s16) r2 * 2);

        f_main_80104DEC_ApplyPan(buf[0], buf[1]);
    }
}
