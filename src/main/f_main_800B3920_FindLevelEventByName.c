#include "../../include/types.h"

extern s16 D_8011FA14;     /* not yet decompiled: level-event entry count, see f_init_80164FDC_CompileLevelEvents */
extern void *D_8013C030[]; /* not yet decompiled: level-event entry pointer table */

extern s32 func_800F8248(char *str, char *name); /* not yet decompiled: guess: strcmp-like name match */

/* guess: linear-searches the level-event table (built by
   f_init_80164FDC_CompileLevelEvents) for an entry whose name field
   (+0x14) matches `name` via func_800F8248, returning that entry (as an
   s32) or 0 if not found / the table is empty. */
s32 f_main_800B3920_FindLevelEventByName(char *name) {
    s32 i;

    for (i = 0; i < D_8011FA14; i++) {
        void *e = D_8013C030[i];

        if (func_800F8248(*(char **) ((u8 *) e + 0x14), name) == 0) {
            return (s32) e;
        }
    }

    return 0;
}
