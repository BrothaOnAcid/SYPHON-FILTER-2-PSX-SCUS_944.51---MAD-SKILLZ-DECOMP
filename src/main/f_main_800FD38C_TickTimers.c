#include "../../include/types.h"

/* Audio-sequencer state and channel records (PSYQ library block, uses its
   own gp base 0x8012EC64). */
extern AudioSeqState g_main_8012F41C_AudioSeq;
extern Rec48 g_main_801412C0_Rec48[];   /* 24 channel records, 0x48 bytes each */

/* Channel timer tick scan (called once per audio poll, before the 24-channel
   loop in f_main_80104B40_PollSequencer). For each active channel whose
   countdown (rec.unk08) is non-zero, decrement it; when it hits 0, set the
   channel's bit in AudioSeqState.pending and fire the channel callback
   (rec.unk44, cmd 3) if present. */
void f_main_800FD38C_TickTimers(void) {
    AudioSeqState *s = &g_main_8012F41C_AudioSeq;
    u32 pend = 0;
    s32 i;

    for (i = 0; i < 24; i++) {
        if (g_main_801412C0_Rec48[i].unk00 == 1 && g_main_801412C0_Rec48[i].unk08 != 0) {
            g_main_801412C0_Rec48[i].unk08--;
            if (g_main_801412C0_Rec48[i].unk08 == 0) {
                pend |= 1u << i;
                if (g_main_801412C0_Rec48[i].unk44 != 0)
                    g_main_801412C0_Rec48[i].unk44(i, g_main_801412C0_Rec48[i].unk40, 3);
            }
        }
    }

    s->pending |= pend;
}
