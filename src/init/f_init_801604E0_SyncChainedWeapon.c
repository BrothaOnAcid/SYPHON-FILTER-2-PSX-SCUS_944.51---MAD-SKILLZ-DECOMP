#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */
extern void **g_main_8011EEFC_ObjArray;       /* array of object pointers */

extern void func_8002CED4(void *a0, s32 a1); /* not yet decompiled */

/* arg0: same weapon-holder object as f_init_80161E28_UpdateWeaponTrack
   (weaponId at +0x2, pointer at +0x8). Follows the holder's weapon's
   chainId (set by f_init_8015E7AC_ResolveChainedWeapon); if present and the
   chained weapon's mode (unk2A) is 0x72, resets the chained object's own
   pointer-chain via func_8002CED4 and mirrors its +0xC value onto arg0's. */
void f_init_801604E0_SyncChainedWeapon(void *arg0) {
    WeaponDef *def = &g_main_8011EEF8_WeaponDefs[*(s16 *) ((u8 *) arg0 + 0x2)];
    s16 chainId = def->chainId;

    if (chainId != -1 && g_main_8011EEF8_WeaponDefs[chainId].unk2A == 0x72) {
        void *obj = g_main_8011EEFC_ObjArray[chainId];
        u8 *p = *(u8 **) ((u8 *) obj + 0x8);
        u8 *self = *(u8 **) ((u8 *) arg0 + 0x8);

        func_8002CED4(p + 0xC, 0);
        func_8002CED4(self + 0xC, *(s32 *) (p + 0xC));
    }
}
