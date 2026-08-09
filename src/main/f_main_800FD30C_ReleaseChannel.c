#include "../../include/types.h"

/* Audio-sequencer state and channel records (PSYQ library block, uses its
   own gp base 0x8012EC64). */
extern AudioSeqState g_main_8012F41C_AudioSeq;
extern Rec48 g_main_801412C0_Rec48[];   /* 24 channel records, 0x48 bytes each */

/* Release channel `i`: unless the voice is a special one (rec.unk28 == 0x7F),
   mark the channel inactive (rec.unk00 = 0); fire the channel callback
   (rec.unk44, cmd 2) if present; then clear the channel's bit from
   AudioSeqState.flagsB (0x8012F438). Called from the sequencer poll when a
   channel is reaped. */
void f_main_800FD30C_ReleaseChannel(s32 i) {
    AudioSeqState *s = &g_main_8012F41C_AudioSeq;
    Rec48 *rec = &g_main_801412C0_Rec48[i];

    if (rec->unk28 != 0x7F)
        rec->unk00 = 0;
    if (rec->unk44 != 0)
        rec->unk44(i, rec->unk40, 2);

    s->flagsB &= ~(1u << i);
}
