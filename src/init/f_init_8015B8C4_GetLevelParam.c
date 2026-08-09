#include "../../include/types.h"

extern u16 g_main_8012B02C_LevelId;  /* guess: current level/map id (main-side global) */

/* Returns a fixed constant selected by the current level id: 0=0x1F0,
   7=0x1C8, 9 or 0x28=0x1B8, everything else=0x188. Purpose unconfirmed. */
s32 f_init_8015B8C4_GetLevelParam(void) {
    s16 level = (s16) g_main_8012B02C_LevelId;

    switch (level) {
    case 7:
        return 0x1C8;
    case 0:
        return 0x1F0;
    case 9:
    case 0x28:
        return 0x1B8;
    default:
        return 0x188;
    }
}
