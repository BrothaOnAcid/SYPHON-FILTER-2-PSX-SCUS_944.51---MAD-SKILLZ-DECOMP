#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */
extern ObjRecord **g_main_8011EEFC_ObjArray;  /* array of object pointers */

/* Sibling to f_init_8015E7AC_ResolveChainedWeapon/f_init_801604E0_SyncChainedWeapon
   (both consume/produce WeaponDef.chainId), but walks a different link:
   follows arg0's weapon's `unk30` objSlot chain up to 2 hops looking for a
   WeaponDef whose `unk2A` is 0x5B; if found, stores that id as arg0's
   weapon's chainId. Stops early if a hop's live object exists with a mode
   other than 8, if the hop limit is hit, or if the chain ends (-1). */
void f_init_8015F314_ResolveObjChain(ObjRecord *arg0) {
    s16 weaponId = arg0->weaponId;
    s16 idx = g_main_8011EEF8_WeaponDefs[weaponId].unk30;
    s32 hops = 1;

    if (idx == -1)
        return;

    for (;;) {
        WeaponDef *def = &g_main_8011EEF8_WeaponDefs[idx];
        ObjRecord *obj = g_main_8011EEFC_ObjArray[idx];

        if (def->unk2A == 0x5B) {
            g_main_8011EEF8_WeaponDefs[weaponId].chainId = idx;
            return;
        }

        if (obj != 0 && obj->mode != 8)
            return;

        hops++;
        if (hops >= 3)
            return;

        idx = def->unk30;
        if (idx == -1)
            return;
    }
}
