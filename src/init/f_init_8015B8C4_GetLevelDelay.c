#include "../../include/types.h"

extern s16 D_8012B02C; /* not yet decompiled: current level id, see f_init_8015B96C_LoadWithFlag */

/* guess: looks up a fixed tick/delay value by level id. */
s32 f_init_8015B8C4_GetLevelDelay(void) {
    switch (D_8012B02C) {
        case 0:
            return 0x1F0;
        case 7:
            return 0x1C8;
        case 9:
        case 0x28:
            return 0x1B8;
        default:
            return 0x188;
    }
}
