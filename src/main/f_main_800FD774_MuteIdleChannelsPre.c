#include "../../include/types.h"

/* Not in asm/800.s: 0x800FD774 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c.

   Byte-for-byte identical to f_main_800FD610_MuteIdleChannels except it
   does NOT call func_80105140 to set the muting gate flag first. Per
   knowledge.txt, f_main_800FD5A0_InvokeHookAndChain calls this THEN
   f_main_800FD610_MuteIdleChannels - so the mute is applied once here,
   then redundantly reapplied (with the gate now set) by the other
   function; kept as two separate functions rather than merged, matching
   the real call graph. */

extern Rec48 g_main_801412C0_Rec48[];
extern s32 f_main_800FCE28_SetStopCallback(void (*cb)(void));
extern void f_main_800FCE58_FlushStopCallback(void);
extern void f_main_8010515C_SetChannelExtra(s32 channel, s16 a1, s16 a2);

void f_main_800FD774_MuteIdleChannelsPre(void) {
    s32 i;

    f_main_800FCE28_SetStopCallback(0);

    for (i = 0; i < 0x18; i++) {
        Rec48 *rec = &g_main_801412C0_Rec48[i];

        if (rec->unk00 == 1 && rec->unk10 < 0x10) {
            f_main_8010515C_SetChannelExtra(i, 0, 0);
            rec->unk20 |= 2;
        }
    }

    f_main_800FCE58_FlushStopCallback();
}
