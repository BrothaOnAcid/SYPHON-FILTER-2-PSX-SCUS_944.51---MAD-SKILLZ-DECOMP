#include "../../include/types.h"

/* PSYQ audio-sequencer state; gp-relative accesses here use the library's
   own gp base 0x8012EC64, not main's gp 0x8011EC64. */
extern AudioSeqState g_main_8012F41C_AudioSeq;

/* --- libc-style externs (not yet decompiled) --- */
extern void f_main_80104B40_PollSequencer(void);

/* Companion to f_main_800FCE28_SetStopCallback: fires and clears any queued
   stop callback (cb2), then clears the "stop-all" flag (unk08). If unk00 is
   set, forces one extra poll cycle and clears it. */
void f_main_800FCE58_FlushStopCallback(void) {
    void (*cb)(void) = g_main_8012F41C_AudioSeq.cb2;

    g_main_8012F41C_AudioSeq.unk08 = 0;
    if (cb != 0) {
        g_main_8012F41C_AudioSeq.cb2 = 0;
        cb();
    }

    if (g_main_8012F41C_AudioSeq.unk00 != 0) {
        f_main_80104B40_PollSequencer();
        g_main_8012F41C_AudioSeq.unk00 = 0;
    }
}
