#include "../../include/types.h"

extern void **g_main_8011EEFC_ObjArray;
extern AmmoUser *g_main_8012A574_Player;
extern AmmoUser *g_main_8012AAD0_Player;

/* If ObjArray[(s16)weaponId] is one of the two player objects, sets bit
   0x10 in the word at player->hud + 0x34. */
void f_main_8003FC98_FlagPlayerHudDirty(s32 weaponId) {
    AmmoUser *obj = (AmmoUser *) g_main_8011EEFC_ObjArray[(s16) weaponId];

    if (obj == g_main_8012A574_Player || obj == g_main_8012AAD0_Player) {
        u32 *flags = (u32 *) ((u8 *) obj->hud + 0x34);
        *flags |= 0x10;
    }
}
