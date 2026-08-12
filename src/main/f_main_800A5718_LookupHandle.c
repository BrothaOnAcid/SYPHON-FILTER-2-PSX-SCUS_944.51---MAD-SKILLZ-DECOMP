#include "../../include/types.h"

/* Guessed: fixed 64-entry handle table (all-zero/BSS at load), stride
   0x1C - id's low byte is the table index, high byte a type/category tag
   stored at each entry's +0x15. */
extern u8 g_main_80137B04_HandleTable[];

/* Looks up `id`'s low byte as an index into g_main_80137B04_HandleTable;
   returns the entry pointer only if its +0x15 tag matches id's high byte
   AND the index is < 0x40, else NULL. */
void *f_main_800A5718_LookupHandle(u32 id) {
    u32 idx = id & 0xFF;
    u8 *entry = g_main_80137B04_HandleTable + idx * 0x1C;

    if ((id >> 8) != entry[0x15]) {
        return 0;
    }
    return (idx < 0x40) ? entry : 0;
}
