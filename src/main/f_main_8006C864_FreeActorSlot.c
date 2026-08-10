#include "../../include/types.h"

extern u16 g_main_8011F7EC_ActiveCount; /* guess: number of live records in g_main_8011F80C_RecordTable */
extern u8 *g_main_8011F7F0_SlotMap;     /* guess: id -> compacted-table-index map, 0xFF = free */
extern SlotRecord *g_main_8011F80C_RecordTable;
extern u16 g_main_8011F834_FreeCount;   /* guess: number of active (non-free) entries at the front of the id space */

/* guess: releases `rec` (identified by its own id, rec->unk31) back to the
   pool. Classic swap-remove over an id->index map: the record currently
   occupying the about-to-be-freed tail slot gets remapped onto the freed
   id, its own back-pointer (unk31) is updated to that id, and the tail
   slot is marked free (0xFF). */
void f_main_8006C864_FreeActorSlot(SlotRecord *rec) {
    u16 freeCount = --g_main_8011F834_FreeCount;
    u8 movedIndex;

    --g_main_8011F7EC_ActiveCount;

    movedIndex = g_main_8011F7F0_SlotMap[freeCount];
    g_main_8011F7F0_SlotMap[rec->unk31] = movedIndex;

    g_main_8011F80C_RecordTable[movedIndex].unk31 = rec->unk31;
    g_main_8011F7F0_SlotMap[freeCount] = 0xFF;
}
