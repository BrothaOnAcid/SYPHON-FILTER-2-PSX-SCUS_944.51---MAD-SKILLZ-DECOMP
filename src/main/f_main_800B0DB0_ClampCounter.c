#include "../../include/types.h"

extern void **g_main_8011EEFC_ObjArray;

/* Clamps g_main_8011EEFC_ObjArray[weaponId]->unk18->unk8 (a positive s16
   counter) down to 0x7FFF if it's currently greater than 0 - i.e. a no-op
   unless the counter has somehow exceeded the s16 positive range. */
void f_main_800B0DB0_ClampCounter(s16 weaponId) {
    AmmoUser *user = (AmmoUser *) g_main_8011EEFC_ObjArray[weaponId];
    u8 *sub = (u8 *) user->unk18;
    s16 *counter = (s16 *) (sub + 8);

    if (*counter > 0) {
        *counter = 0x7FFF;
    }
}
