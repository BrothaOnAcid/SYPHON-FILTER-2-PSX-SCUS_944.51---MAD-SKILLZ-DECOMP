#include "../../include/types.h"

extern Rec48 g_main_801412C0_Rec48[];   /* 24 channel records, 0x48 bytes each */

/* --- libc-style externs (not yet decompiled) --- */
extern void f_main_80105A9C_QueueOrApplyMask(s32 cmd, u32 arg);
                                                  f_main_800FD258_ReapPending */

/* Stops channel `i`: tells the audio engine to silence it (cmd 0, channel
   bitmask), unconditionally clears rec.unk00 (unlike
   f_main_800FD30C_ReleaseChannel, no rec.unk28 == 0x7F guard), then fires
   the channel callback (rec.unk44, cmd 4) if present. */
void f_main_800FD2A4_StopChannel(s32 i) {
    Rec48 *rec = &g_main_801412C0_Rec48[i];

    f_main_80105A9C_QueueOrApplyMask(0, 1u << i);
    rec->unk00 = 0;
    if (rec->unk44 != 0)
        rec->unk44(i, rec->unk40, 4);
}
