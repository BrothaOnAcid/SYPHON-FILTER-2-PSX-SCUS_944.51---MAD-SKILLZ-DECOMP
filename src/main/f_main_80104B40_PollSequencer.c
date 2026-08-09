#include "../../include/types.h"

/* Audio-sequencer state and channel tables (PSYQ library block, uses its
   own gp base 0x8012EC64). */
extern AudioSeqState g_main_8012F41C_AudioSeq;
extern Rec48 g_main_801412C0_Rec48[];   /* 24 channel records, 0x48 bytes each */
extern u16 g_main_80141A70_SeqVal[];    /* per-channel u16 (0x80141A70) */
extern u8 g_main_80141A58_SeqCount[];   /* per-channel u8  (0x80141A58) */

/* --- not yet decompiled (PSYQ audio runtime, blocked region) --- */
extern void func_80105300(void);   /* gp-swap: save caller gp, load library gp */
extern void func_80105318(void);   /* gp-swap: restore caller gp */
extern void f_main_800FD30C_ReleaseChannel(s32 i);  /* release channel i */
extern s16 func_80107C0C(s32 i, u16 *out); /* read channel status word */
extern void f_main_80104E50_TickSchedule(void);     /* sequencer tail helper */
extern void f_main_800FD258_ReapPending(void);  /* decompiled pending reaper */
extern void f_main_800FD1CC_ActivateChannels(void);   /* pending reaper (companion) */
extern void f_main_800FD38C_TickTimers(void);   /* channel timer tick scan */
extern void f_main_800FD430_RequeueChannel(s32 i); /* requeue channel i */

/* Main audio-sequencer poll. Reentrancy-guarded (AudioSeqState.busy /
   unk54); when the stop-all flag (unk08) is set, raises unk00 and bails.
   Otherwise increments the poll counter, ticks channel timers
   (func_800FD38C), then scans the 24 channel records: for each active
   (unk00 == 1), not-in-use (mask), zero-status channel either reaps it
   (flagsB bit -> f_main_800FD30C_ReleaseChannel) or drives the per-channel
   countdown
   (g_main_80141A58_SeqCount) / sequence value (g_main_80141A70_SeqVal),
   requeueing via func_800FD430 when the countdown hits 0. Ends by running
   the tail callbacks (cb0/cb1) and the two pending reapers. */
s32 f_main_80104B40_PollSequencer(void) {
    AudioSeqState *s = &g_main_8012F41C_AudioSeq;
    Rec48 *rec;
    u16 *p16;
    u8 *p8;
    u16 status;
    u32 bit;
    s32 i;

    func_80105300();

    if (s->busy != 0 || s->unk54 != 0) {
        func_80105318();
        return 0;
    }

    s->busy = 1;
    if (s->unk08 != 0) {
        s->unk00 = 1;
        s->busy = 0;
        func_80105318();
        return 0;
    }

    s->count++;
    f_main_800FD38C_TickTimers();

    p16 = g_main_80141A70_SeqVal;
    p8 = g_main_80141A58_SeqCount;
    rec = g_main_801412C0_Rec48;

    for (i = 0; i < 24; i++) {
        bit = rec[i].unk00 << i;
        if (rec[i].unk00 != 1)
            continue;
        if (s->mask & bit)
            continue;
        status = func_80107C0C(i, &status);
        if (status != 0)
            continue;
        if (s->flagsB & bit) {
            f_main_800FD30C_ReleaseChannel(i);
            continue;
        }
        if (!(s->flagsA & bit))
            continue;
        if (p16[i] == 0)
            p8[i] = 8;
        else {
            p8[i]--;
            if (p8[i] == 0)
                f_main_800FD430_RequeueChannel(i);
        }
        p16[i] = status;
    }

    f_main_80104E50_TickSchedule();
    if (s->cb0 != 0)
        s->cb0();
    if (s->cb1 != 0)
        s->cb1();
    f_main_800FD258_ReapPending();
    f_main_800FD1CC_ActivateChannels();
    s->busy = 0;

    func_80105318();
    return 0;
}
