#include "../../include/types.h"

/* Audio-sequencer state and channel records (PSYQ library block, uses its
   own gp base 0x8012EC64). */
extern AudioSeqState g_main_8012F41C_AudioSeq;
extern Rec48 g_main_801412C0_Rec48[];   /* 24 channel records, 0x48 bytes each */

/* Requeue channel `i`: if the channel is active (unk00 == 1) and has a
   callback, fire it (rec.unk44, cmd 3); then clear the channel's bit from
   AudioSeqState.mask (0x8012F42C) and set it in AudioSeqState.pending
   (0x8012F430). Called from the sequencer poll when a channel countdown
   reaches 0. */
void f_main_800FD430_RequeueChannel(s32 i) {
    AudioSeqState *s = &g_main_8012F41C_AudioSeq;
    Rec48 *rec = &g_main_801412C0_Rec48[i];
    u32 bit = 1u << i;

    if (rec->unk00 == 1) {
        if (rec->unk44 != 0)
            rec->unk44(i, rec->unk40, 3);
    }

    if (s->mask & bit)
        s->mask &= ~bit;
    s->pending |= bit;
}
