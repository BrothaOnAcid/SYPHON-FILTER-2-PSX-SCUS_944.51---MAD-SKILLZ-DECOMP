#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */

extern s16 func_800459E4(s16 id, s16 table); /* not yet decompiled: remaps `id` through table `table` */

/* arg0: same weapon-holder object as f_init_80161E28_UpdateWeaponTrack
   (weaponId at +0x2, mode byte at +0x26). Resolves the holder's weapon id
   through remap table 0x1E; if that changed it (and the holder isn't in
   mode 2), resolves again through table 0x5B and, if that changed it too,
   stores the final id as the weapon's chainId (see
   f_init_801604E0_SyncChainedWeapon, which consumes it). */
void f_init_8015E7AC_ResolveChainedWeapon(void *arg0) {
    s16 weaponId = *(s16 *) ((u8 *) arg0 + 0x2);
    s16 remapped = func_800459E4(weaponId, 0x1E);

    if (weaponId != remapped && *((u8 *) arg0 + 0x26) != 2) {
        s16 remapped2 = func_800459E4(remapped, 0x5B);
        if (remapped2 != remapped) {
            g_main_8011EEF8_WeaponDefs[weaponId].chainId = remapped2;
        }
    }
}
