#include "../../include/types.h"

/* Not in asm/800.s: 0x800FD6A4 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c. */

extern Rec48 g_main_801412C0_Rec48[];
extern s32 f_main_800FCE28_SetStopCallback(void (*cb)(void));
extern void f_main_800FCE58_FlushStopCallback(void);
extern s32 f_main_800FBF64_ScaleByVolumeCurve(s16 raw, s32 index);
extern void f_main_8010515C_SetChannelExtra(s32 channel, s16 a1, s16 a2);
extern void func_80105150(void);   /* clears the "muting" gate flag, gp+0x81c */

/* guess: counterpart to f_main_800FD610_MuteIdleChannels - restores the
   volume-scaled pan for every voice tagged with unk20 bit 1 (skipping
   ones whose unk10 threshold is still below 0x10), clears the tag, then
   releases the muting gate. */
void f_main_800FD6A4_RestoreTaggedChannels(void) {
    s32 i;

    f_main_800FCE28_SetStopCallback(0);

    for (i = 0; i < 0x18; i++) {
        Rec48 *rec = &g_main_801412C0_Rec48[i];

        if ((rec->unk20 & 2) && rec->unk10 >= 0x10) {
            s16 l = f_main_800FBF64_ScaleByVolumeCurve(rec->unk0C, rec->unk30);
            s16 r = f_main_800FBF64_ScaleByVolumeCurve(rec->unk0E, rec->unk30);

            f_main_8010515C_SetChannelExtra(i, l, r);
            rec->unk20 &= ~2;
        }
    }

    f_main_800FCE58_FlushStopCallback();
    func_80105150();
}
