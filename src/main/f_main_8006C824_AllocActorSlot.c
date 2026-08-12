#include "../../include/types.h"

extern u16 g_main_8011F7EC_ActiveCount; /* guess: number of live records in g_main_8011F80C_RecordTable */
extern u8 *g_main_8011F7F0_SlotMap;     /* guess: id -> compacted-table-index map, 0xFF = free */
extern u16 g_main_8011F834_FreeCount;   /* guess: number of active (non-free) entries at the front of the id space */

/* guess: counterpart to f_main_8006C864_FreeActorSlot - assigns `rec` the
   next free id (g_main_8011F834_FreeCount, taken before incrementing),
   records rec's already-set compacted-table index (rec->unk30) into
   g_main_8011F7F0_SlotMap[id], tags rec->unk31 with that id, and bumps
   both the id counter and the active-record counter. */
void f_main_8006C824_AllocActorSlot(SlotRecord *rec) {
    u16 id = g_main_8011F834_FreeCount;

    g_main_8011F7F0_SlotMap[id] = rec->unk30;
    rec->unk31 = (u8) g_main_8011F834_FreeCount;
    g_main_8011F834_FreeCount++;
    g_main_8011F7EC_ActiveCount++;
}
