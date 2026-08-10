#include "../../include/types.h"

extern s16 g_main_8011F7C4_unk; /* guess: some "unlimited"/availability flag, checked for weapon id 0x12 */

/* guess: reads the two ammo-slot halves for `weaponId` out of the
   AmmoUser's hud block (AmmoUser.hud + 0x44, stride 4, see AmmoSlot).
   Weapon id 0x12 (melee?) is special-cased: no per-slot lookup, `outA` is
   just 1 when g_main_8011F7C4_unk == 0 else 0, `outB` always 0. Either
   output pointer may be NULL. */
void f_main_800649BC_GetAmmoCounts(AmmoUser *user, s32 weaponId, s32 *outA, s32 *outB) {
    AmmoSlot *slot;

    if (weaponId == 0x12) {
        if (outA != 0)
            *outA = (g_main_8011F7C4_unk == 0) ? 1 : 0;
        if (outB != 0)
            *outB = 0;
        return;
    }

    slot = (AmmoSlot *) ((u8 *) user->hud + 0x44) + weaponId;
    if (outA != 0)
        *outA = slot->current;
    if (outB != 0)
        *outB = slot->max;
}
