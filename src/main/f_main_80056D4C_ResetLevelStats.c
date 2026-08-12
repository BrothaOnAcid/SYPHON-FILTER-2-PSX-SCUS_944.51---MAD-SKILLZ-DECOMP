#include "../../include/types.h"

/* Guessed: per-level (0x2A/42 levels, matching f_main_8002A338_LoadLevel's
   level-id clamp) stats table, stride 0x2C bytes; only the leading s32
   field is known/reset here. */
extern u8 g_main_8012E424_LevelStats[];
/* Guessed: per-level status word, -1 = "not yet visited/unset". */
extern s32 g_main_8012EB3C_LevelStatus[];

/* Resets all 42 level-stats/status entries: zeroes each stats record's
   leading word and marks each status entry unset (-1). */
void f_main_80056D4C_ResetLevelStats(void) {
    s32 i;

    for (i = 0; i < 0x2A; i++) {
        *(s32 *) (g_main_8012E424_LevelStats + i * 0x2C) = 0;
        g_main_8012EB3C_LevelStatus[i] = -1;
    }
}
