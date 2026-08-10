#include "../../include/types.h"

extern LookupTable4 g_main_80138680_unk;

/* guess: scans the live entries (0..count) of g_main_80138680_unk for a
   row whose keyA matches `a` (or is -1, wildcard) and whose keyB matches
   `b` (or is -1), returning its value; 0 if no row matches. */
s32 f_main_800AA658_LookupValue(s32 a, s32 b) {
    s32 i;

    for (i = 0; i < g_main_80138680_unk.count; i++) {
        if ((g_main_80138680_unk.keyA[i] == (s16) a || g_main_80138680_unk.keyA[i] == -1) &&
            (g_main_80138680_unk.keyB[i] == (s16) b || g_main_80138680_unk.keyB[i] == -1))
            return g_main_80138680_unk.value[i];
    }

    return 0;
}
