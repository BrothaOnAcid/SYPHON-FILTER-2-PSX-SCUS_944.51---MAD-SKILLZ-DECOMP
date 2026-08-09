#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs;

extern void func_800467D8(void *arg0, s32 a1);

/* arg0: same weapon-holder/track shape as f_init_80161E28_UpdateWeaponTrack
   (id at +0x2, a pointer at +0x18 whose +0x8 gets cleared, and a flags byte
   at +0x27). */
void f_init_8015E730_ClearTrackSlot(void *arg0) {
    s16 id = *(s16 *) ((u8 *) arg0 + 0x2);

    if (g_main_8011EEF8_WeaponDefs[id].unk30 != -1) {
        func_800467D8(arg0, 0);
        return;
    }

    *(s16 *) (*(u8 **) ((u8 *) arg0 + 0x18) + 0x8) = -1;
    *((u8 *) arg0 + 0x27) |= 0x2;
}
