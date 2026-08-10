#include "../../include/types.h"

extern IdleTable *g_main_8011F5B8_unk;

/* guess: returns the index of the first entry in
   g_main_8011F5B8_unk[id].items whose record has unk8/9/A all <= 0
   ("idle"), or -1 if none. */
s32 f_main_80080428_FindIdleSlot(s32 id) {
    IdleTable *table = &g_main_8011F5B8_unk[id];
    s32 i;

    for (i = 0; i < table->count; i++) {
        IdleRecord *rec = *(IdleRecord **) (table->items + i);

        if (rec->unk8 <= 0 && rec->unk9 <= 0 && rec->unkA <= 0)
            return i;
    }

    return -1;
}
