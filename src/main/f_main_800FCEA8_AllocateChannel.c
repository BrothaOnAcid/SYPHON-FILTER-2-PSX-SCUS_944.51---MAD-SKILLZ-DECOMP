#include "../../include/types.h"

/* Not in asm/800.s: 0x800FCEA8 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c.

   Takes 3 args, not the 2 originally guessed in
   f_main_800F9F68_PlaySongNode's extern (fixed there too): `cb` there is
   the address of the previously-"dead" fragment at 0x800FA7AC (now
   decompiled as f_main_800FA7AC_ClearChildMaskBit), confirming it's a
   ChannelCb reached only via this stored function pointer, never a direct
   call. */

extern Rec48 g_main_801412C0_Rec48[];
extern s32 f_main_80104D30_GetSeqCount(void);

/* guess: allocates a channel record for `voiceId`. First scans for a free
   slot (unk00 == 0); if none, picks the lowest-priority active slot
   (lowest unk28 "voice id", tie-broken by lowest unk04) to steal - but
   refuses (returns -1) if the new voice's priority isn't higher than the
   victim's, or the victim is protected (unk28 == 0x7F). When stealing, an
   existing callback on the victim fires once with cmd 1 first. Either way
   records unk28/unk40/unk44 = (voiceId, arg, cb) and unk04 = a fresh
   sequence count (tie-break generation for future steals); does NOT set
   unk00 = 1 itself - that's left to the caller. Returns the allocated
   channel index, or -1 on failure. */
s32 f_main_800FCEA8_AllocateChannel(u8 voiceId, void *arg, ChannelCb cb) {
    s32 i;
    s32 chosen = -1;

    for (i = 0; i < 0x18; i++) {
        if (g_main_801412C0_Rec48[i].unk00 == 0) {
            chosen = i;
            break;
        }
    }

    if (chosen == -1) {
        s32 best = 0;

        for (i = 1; i < 0x18; i++) {
            if ((s32) g_main_801412C0_Rec48[i].unk28 < (s32) g_main_801412C0_Rec48[best].unk28) {
                best = i;
            } else if (g_main_801412C0_Rec48[i].unk28 == g_main_801412C0_Rec48[best].unk28) {
                if (g_main_801412C0_Rec48[i].unk04 < g_main_801412C0_Rec48[best].unk04) {
                    best = i;
                }
            }
        }

        if ((s32) voiceId < (s32) g_main_801412C0_Rec48[best].unk28) {
            return -1;
        }
        if (g_main_801412C0_Rec48[best].unk28 == 0x7F) {
            return -1;
        }

        if (g_main_801412C0_Rec48[best].unk44 != 0) {
            g_main_801412C0_Rec48[best].unk44(best, g_main_801412C0_Rec48[best].unk40, 1);
        }

        g_main_801412C0_Rec48[best].unk00 = 2;
        g_main_801412C0_Rec48[best].unk04 = f_main_80104D30_GetSeqCount();
        chosen = best;
    } else {
        g_main_801412C0_Rec48[chosen].unk00 = 0;
        g_main_801412C0_Rec48[chosen].unk04 = f_main_80104D30_GetSeqCount();
    }

    g_main_801412C0_Rec48[chosen].unk28 = voiceId;
    g_main_801412C0_Rec48[chosen].unk40 = PTR_U32(arg);
    g_main_801412C0_Rec48[chosen].unk44 = cb;

    return chosen;
}
