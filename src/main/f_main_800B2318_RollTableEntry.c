#include "../../include/types.h"

extern void *g_main_8011FA04_TableHi; /* selected when arg0 & 0x8000 */
extern void *g_main_8011FA1C_TableLo; /* selected otherwise */

extern s32 func_800F41F0(void); /* PS1 BIOS rand() (A0 table index 0x2F) */

/* arg0: low byte selects a slot (stride 2) in the chosen table's +0xC
   array, high bit selects which table; arg1: modulus for the random roll. */
void f_main_800B2318_RollTableEntry(s32 arg0, u32 arg1) {
    void *table = (arg0 & 0x8000) ? g_main_8011FA04_TableHi : g_main_8011FA1C_TableLo;
    s16 *slots = *(s16 **) ((u8 *) table + 0xC);

    slots[arg0 & 0xFF] = (s16) (func_800F41F0() % arg1);
}
