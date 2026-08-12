#include "../../include/types.h"

/* --- not yet decompiled --- */
extern u8 *func_8007C5E0(void);

/* Guessed: per-index status table, cleared (-1) here. */
extern s16 g_main_8011F8C8_SlotTable[];

/* If func_8007C5E0 returns a "current" object, clears the slot table entry
   at that object's ->unk25 index. */
void f_main_8007C7A0_ClearSlotEntry(void) {
    u8 *cur = func_8007C5E0();

    if (cur != 0) {
        g_main_8011F8C8_SlotTable[cur[0x25]] = -1;
    }
}
