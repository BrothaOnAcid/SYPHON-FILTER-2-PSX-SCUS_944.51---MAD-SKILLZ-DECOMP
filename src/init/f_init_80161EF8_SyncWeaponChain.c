#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */

extern ObjRecord *f_main_80045B98_FindChainedObj(s16 objSlot, s16 mode);
extern void f_init_80161E28_UpdateWeaponTrack(void *arg0);

/* guess: if `target`'s linked node (+0x18) doesn't have its +0x8 halfword
   set to the "unset" sentinel 0x7FFF, resolves another weapon (via
   f_main_80045B98_FindChainedObj(weaponId, 5)) and, if it differs from
   `target`'s own weapon id, chains the two together: stamps the other
   weapon's id into g_main_8011EEF8_WeaponDefs[weaponId].chainId and runs
   f_init_80161E28_UpdateWeaponTrack on that WeaponDef. */
void f_init_80161EF8_SyncWeaponChain(ObjRecord *target) {
    u8 *t = (u8 *) target;
    void *linked = *(void **) (t + 0x18);

    if (*(s16 *) ((u8 *) linked + 8) != 0x7FFF) {
        s16 weaponId = *(s16 *) (t + 2);
        ObjRecord *other = f_main_80045B98_FindChainedObj(weaponId, 5);
        s16 otherId = *(s16 *) ((u8 *) other + 2);

        if (otherId != weaponId) {
            WeaponDef *def = &g_main_8011EEF8_WeaponDefs[weaponId];

            def->chainId = otherId;
            f_init_80161E28_UpdateWeaponTrack(def);
        }
    }
}
