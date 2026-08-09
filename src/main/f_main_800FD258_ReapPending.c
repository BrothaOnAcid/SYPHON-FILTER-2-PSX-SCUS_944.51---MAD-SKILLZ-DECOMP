#include "../../include/types.h"

/* Audio-sequencer state (PSYQ library block). The library uses its own gp
   base 0x8012EC64 (set at 0x800F8614), so gp-relative offsets resolve into
   0x8012F4xx: +0x7CC = 0x8012F430 (pending), +0x7D0 = 0x8012F434 (flagsA),
   +0x7D4 = 0x8012F438 (flagsB). */
extern AudioSeqState g_main_8012F41C_AudioSeq;

/* --- not yet decompiled (PSYQ audio runtime, blocked region) --- */
extern void f_main_80105A9C_QueueOrApplyMask(s32 cmd, u32 arg);

/* Audio/SPU queue reaper: when the pending bitmask is set, wake the audio
   engine (f_main_80105A9C_QueueOrApplyMask command 0, with the pending mask as the second arg -
   see f_main_800FD2A4_StopChannel/f_main_800FD1CC_ActivateChannels for other
   callers of the same function), then move the pending bits out of
   flagsA into flagsB and clear the pending mask. Companion of
   f_main_800FD1CC_ActivateChannels; both are called from the audio-poll
   dispatch at 0x80104CF0. */
void f_main_800FD258_ReapPending(void) {
    u32 pending = g_main_8012F41C_AudioSeq.pending;

    if (pending != 0) {
        f_main_80105A9C_QueueOrApplyMask(0, pending);
        pending = g_main_8012F41C_AudioSeq.pending;
        g_main_8012F41C_AudioSeq.pending = 0;
        g_main_8012F41C_AudioSeq.flagsA &= ~pending;
        g_main_8012F41C_AudioSeq.flagsB |= pending;
    }
}
