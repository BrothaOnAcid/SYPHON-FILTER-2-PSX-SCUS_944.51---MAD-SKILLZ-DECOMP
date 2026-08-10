#include "../../include/types.h"

extern void *g_main_8011FA04_TableHi; /* selected when arg0 & 0x8000, see f_main_800B2318_RollTableEntry */
extern void *g_main_8011FA1C_TableLo; /* selected otherwise */
extern void *g_main_8012099C_unk;     /* guess: current target/actor pointer, +0xC = per-actor status block */
extern s32 g_main_8012A640_DefaultSlotValue; /* guess: fallback value for mode 0 */

/* guess: companion of f_main_800B2318_RollTableEntry - writes a computed
   value into the same table's +0xC s16 array. `mode` selects the source:
   0 = a global default, 1/else = abs(status->unk1C or unk14) >> 5. */
void f_main_800AFB68_SetTableSlot(s32 id, s32 mode) {
    void *status = *(void **) ((u8 *) g_main_8012099C_unk + 0xC);
    void *table = (id & 0x8000) ? g_main_8011FA04_TableHi : g_main_8011FA1C_TableLo;
    s16 *slots = *(s16 **) ((u8 *) table + 0xC);
    s32 val;

    if (mode == 0) {
        val = g_main_8012A640_DefaultSlotValue;
    } else {
        val = (mode == 1) ? *(s32 *) ((u8 *) status + 0x1C) : *(s32 *) ((u8 *) status + 0x14);
        if (val < 0)
            val = -val;
        val >>= 5;
    }

    slots[id & 0xFF] = (s16) val;
}
