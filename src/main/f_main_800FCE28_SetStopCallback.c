#include "../../include/types.h"

/* PSYQ audio-sequencer state; this function is in the blocked region
   (0x800EFD3C-0x80168298) that uses its own gp base 0x8012EC64, not main's
   gp 0x8011EC64 - see g_main_8012F41C_AudioSeq. */
extern AudioSeqState g_main_8012F41C_AudioSeq;

/* First call: marks the "stop-all" shortcut flag (unk08) and returns 1.
   Later calls: if `cb` is non-NULL, stores it into cb2 for later use;
   returns 0. */
s32 f_main_800FCE28_SetStopCallback(void (*cb)(void)) {
    if (g_main_8012F41C_AudioSeq.unk08 == 0) {
        g_main_8012F41C_AudioSeq.unk08 = 1;
        return 1;
    }

    if (cb != 0)
        g_main_8012F41C_AudioSeq.cb2 = cb;

    return 0;
}
