#include "../../include/types.h"

extern void *g_main_8012A574_Player; /* guess: player 1 character object */
extern void *g_main_8012AAD0_Player; /* guess: player 2 character object */
extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */

/* For player objects, seeds flags1 from *(hud+0xCC) (see
   f_main_80064398_NotifyEmpty's identical hud+0xCC read); for anyone else,
   seeds it from the weapon's ammoType masked to 6 bits (matches
   f_main_800644AC_HandleAmmo's `ammoType & 0x3F`).

   Then, unless the user is a player, sets bit 0x40 on flags1 if either the
   weapon's mode (unk2A) is 0x41 or its ammoType has bit 0x4000 set (players
   always get the bit set unconditionally). */
void f_init_80160E64_UpdateAmmoDisplayFlags(AmmoUser *user) {
    s32 setBit40;

    if (user == g_main_8012A574_Player || user == g_main_8012AAD0_Player) {
        user->flags1 = (u8) *(s32 *) ((u8 *) user->hud + 0xCC);
    } else {
        user->flags1 = (u8) (g_main_8011EEF8_WeaponDefs[user->weaponId].ammoType & 0x3F);
    }

    if (user == g_main_8012A574_Player || user == g_main_8012AAD0_Player) {
        setBit40 = 1;
    } else {
        WeaponDef *def = &g_main_8011EEF8_WeaponDefs[user->weaponId];
        setBit40 = (def->unk2A == 0x41) || (def->ammoType & 0x4000);
    }

    if (setBit40) {
        user->flags1 |= 0x40;
    }
}
