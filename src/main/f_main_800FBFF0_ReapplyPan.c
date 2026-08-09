#include "../../include/types.h"

/* Not in asm/800.s: 0x800FBFF0 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c. */

extern Rec48 g_main_801412C0_Rec48[];
extern s32 f_main_800FCE28_SetStopCallback(void (*cb)(void));
extern void f_main_800FCE58_FlushStopCallback(void);
extern s32 f_main_800FBF64_ScaleByVolumeCurve(s16 raw, s32 index);
extern void f_main_8010515C_SetChannelExtra(s32 channel, s16 a1, s16 a2);

/* guess: `value` is unused (overwritten before read) - re-broadcasts the
   volume-scaled pan (f_main_800FBF64_ScaleByVolumeCurve) for every active
   voice (Rec48[i].unk00 == 1) currently tagged with logical channel/param
   `index` (Rec48[i].unk30), applying it via
   f_main_8010515C_SetChannelExtra(i, ...). Called from
   f_main_800FBAE4_SetChannelParam after a parameter change. */
void f_main_800FBFF0_ReapplyPan(s32 value, s32 index) {
    s32 i;

    (void) value;

    f_main_800FCE28_SetStopCallback(0);

    for (i = 0; i < 0x18; i++) {
        Rec48 *rec = &g_main_801412C0_Rec48[i];

        if (rec->unk00 == 1 && (s32) rec->unk30 == index) {
            s16 l = f_main_800FBF64_ScaleByVolumeCurve(rec->unk0C, index);
            s16 r = f_main_800FBF64_ScaleByVolumeCurve(rec->unk0E, index);

            f_main_8010515C_SetChannelExtra(i, l, r);
        }
    }

    f_main_800FCE58_FlushStopCallback();
}
