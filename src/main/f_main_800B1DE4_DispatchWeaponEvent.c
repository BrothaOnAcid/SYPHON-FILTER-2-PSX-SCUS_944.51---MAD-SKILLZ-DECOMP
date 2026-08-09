#include "../../include/types.h"

extern void **g_main_8011EEFC_ObjArray;
extern AmmoUser *g_main_8012A574_Player;

extern void func_8002AC88(s32 a0, s32 a1, s16 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);

/* If ObjArray[arg0]'s type byte (+0x26) is 8 or 0x12, use arg0 itself as
   the id (a2); otherwise default to the player's current weaponId. */
void f_main_800B1DE4_DispatchWeaponEvent(s32 arg0, s32 arg1) {
    void *obj = g_main_8011EEFC_ObjArray[arg0];
    u8 type = *((u8 *) obj + 0x26);
    s16 id = (type == 8 || type == 0x12) ? (s16) arg0 : g_main_8012A574_Player->weaponId;

    func_8002AC88(arg1 & 0xFF, 2, id, arg0 & 0xFFFF, 0, 0, 0, 0);
}
