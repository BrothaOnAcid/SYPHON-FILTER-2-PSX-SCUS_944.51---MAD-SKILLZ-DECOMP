#include "../../include/types.h"

/* Zeroes a halfword at +0x40/+0x42, then fills 16 entries of four parallel
   per-channel arrays hung off `base`: byte arrays at +0x44 (0) and +0x54
   (0x7F), halfword arrays at +0x64 (0) and +0x84 (0x2000). Likely related to
   the per-channel countdown/seq arrays noted in knowledge.txt
   (g_main_80141A58 u8 countdowns, g_main_80141A70 u16 seq values), though the
   exact struct this `base` points to is unconfirmed. */
void f_main_800FEAA4_ClearChannelArrays(u8 *base) {
    s32 i;

    *(u16 *) (base + 0x40) = 0;

    for (i = 0; i < 16; i++) {
        base[i + 0x44] = 0;
        base[i + 0x54] = 0x7F;
        *(u16 *) (base + i * 2 + 0x64) = 0;
        *(u16 *) (base + i * 2 + 0x84) = 0x2000;
    }

    *(u16 *) (base + 0x42) = 0;
}
