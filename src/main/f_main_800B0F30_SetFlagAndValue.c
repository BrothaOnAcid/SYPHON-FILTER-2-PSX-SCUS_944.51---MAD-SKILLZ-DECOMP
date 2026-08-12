#include "../../include/types.h"

extern void **g_main_8011EEFC_ObjArray;

/* Sets bit 0x80000 in g_main_8011EEFC_ObjArray[weaponId]->unk1C's +0x4
   flags word, and stores `value` into that same sub-object's +0x10 field. */
void f_main_800B0F30_SetFlagAndValue(s16 weaponId, s16 value) {
    AmmoUser *user = (AmmoUser *) g_main_8011EEFC_ObjArray[weaponId];
    u8 *sub = (u8 *) user->unk1C;

    *(s32 *) (sub + 4) |= 0x80000;
    *(s16 *) (sub + 0x10) = value;
}
