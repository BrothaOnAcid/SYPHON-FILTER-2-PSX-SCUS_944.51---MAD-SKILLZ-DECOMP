#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */
extern void **g_main_8011EEFC_ObjArray;       /* array of object pointers */

void f_init_8015EA58_LevelizeWeaponChain(u32 chainLen, void *firstHolder, void *lastObj);

/* arg0: same weapon-holder object as f_init_80161E28_UpdateWeaponTrack
   (weaponId at +0x2, mode byte at +0x26, flag byte at +0x27). Starting from
   arg0's weapon's chained object (unk30 index), walks the chain of objects
   whose mode is 5, re-pointing each link's WeaponDef.chainId (+0x48) back
   at the previous holder's weapon id as it goes; stops early if arg0's
   +0x27 has bit 2 set. Once the walk ends on a still-valid chain slot,
   hands the whole (count, first, last) run off to
   f_init_8015EA58_LevelizeWeaponChain. */
void f_init_8015EBB8_PropagateWeaponChain(void *arg0) {
    s32 chainIdx = g_main_8011EEF8_WeaponDefs[*(s16 *) ((u8 *) arg0 + 0x2)].unk30;
    void *prev, *cur;
    s32 count;

    if (chainIdx == -1 || (*((u8 *) arg0 + 0x27) & 2)) {
        return;
    }

    prev = arg0;
    cur = g_main_8011EEFC_ObjArray[chainIdx];
    count = 1;

    while (*((u8 *) cur + 0x26) == 5) {
        g_main_8011EEF8_WeaponDefs[chainIdx].chainId = *(s16 *) ((u8 *) prev + 0x2);
        prev = cur;
        chainIdx = g_main_8011EEF8_WeaponDefs[*(s16 *) ((u8 *) cur + 0x2)].unk30;
        cur = g_main_8011EEFC_ObjArray[chainIdx];
        count++;
        if (chainIdx == -1) {
            break;
        }
    }

    if (chainIdx != -1) {
        f_init_8015EA58_LevelizeWeaponChain(count, prev, cur);
    }
}
