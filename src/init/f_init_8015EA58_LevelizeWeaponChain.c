#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */
extern void **g_main_8011EEFC_ObjArray;       /* array of object pointers */
extern void *D_80168AC8[];                    /* not yet decompiled: per-level linked-list heads, indexed by the low byte of unk4 */

extern void func_8015E9C0(void *obj, void *last); /* not yet decompiled */

/* Called by f_init_8015EBB8_PropagateWeaponChain with (chainLen, firstHolder,
   lastObj). lastObj's +0x4 packs a "level" nibble at bits 0xF000 and a list
   index at bits 0xFF. If that level is smaller than chainLen (clamped to
   7): walks the D_80168AC8[listIdx] linked list (node = { void *obj; ...;
   void *next; } at +0x0/+0x8), stamping every node's own WeaponDef.chainId
   with firstHolder's weapon id and bumping the node object's level nibble
   to chainLen. Then walks the weapon-holder chain starting at firstHolder,
   flagging each unflagged holder (+0x27 bit 2) and calling func_8015E9C0 on
   it, following each holder's WeaponDef.chainId until it hits -1. */
void f_init_8015EA58_LevelizeWeaponChain(u32 chainLen, void *firstHolder, void *lastObj) {
    s32 unk4 = *(s32 *) ((u8 *) lastObj + 4);
    u32 level = ((u32) unk4 & 0xF000) >> 12;
    u32 newLevel;
    void *node;
    s16 weaponId;

    if (level >= chainLen) {
        return;
    }

    newLevel = (chainLen >= 8) ? 7 : chainLen;

    node = D_80168AC8[unk4 & 0xFF];
    while (node != 0) {
        void *obj = *(void **) node;
        s16 objWeaponId = *(s16 *) ((u8 *) obj + 2);

        g_main_8011EEF8_WeaponDefs[objWeaponId].chainId = *(u16 *) ((u8 *) firstHolder + 2);
        *(u32 *) ((u8 *) obj + 4) = (*(u32 *) ((u8 *) obj + 4) & 0xFFFF0FFF) | ((newLevel << 12) & 0xF000);

        node = *(void **) ((u8 *) node + 8);
    }

    weaponId = *(s16 *) ((u8 *) firstHolder + 2);
    do {
        void *obj = g_main_8011EEFC_ObjArray[weaponId];
        u8 flags27 = *((u8 *) obj + 0x27);

        if (!(flags27 & 2)) {
            *((u8 *) obj + 0x27) = flags27 | 2;
            func_8015E9C0(obj, lastObj);
        }
        weaponId = g_main_8011EEF8_WeaponDefs[*(s16 *) ((u8 *) obj + 2)].chainId;
    } while (weaponId != -1);
}
