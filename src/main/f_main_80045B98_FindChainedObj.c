#include "../../include/types.h"

extern u8 func_8002D4FC(s16 weaponId, WeaponDef *def); /* not yet decompiled */

extern s32 g_main_8011F564_ObjCount;
extern ObjRecord **g_main_8011EEFC_ObjArray;
extern WeaponDef *g_main_8011EEF8_WeaponDefs;

/* guess: scans active weapon objects for one whose ObjRecord.mode matches
   `mode` and whose WeaponDef.unk30 objArray-slot points back at `objSlot`,
   returning that ObjRecord. Falls back to g_main_8011EEFC_ObjArray[objSlot]
   directly if none is found. */
ObjRecord *f_main_80045B98_FindChainedObj(s16 objSlot, s16 mode) {
    s32 i = 0;
    s32 n = g_main_8011F564_ObjCount;

    while (i < n) {
        if (func_8002D4FC((s16) i, 0) == 0) {
            ObjRecord *entry = g_main_8011EEFC_ObjArray[i];
            if (entry != 0 && entry->mode == mode &&
                g_main_8011EEF8_WeaponDefs[i].unk30 == objSlot) {
                return entry;
            }
        }
        n = g_main_8011F564_ObjCount;
        i++;
    }

    return g_main_8011EEFC_ObjArray[objSlot];
}
