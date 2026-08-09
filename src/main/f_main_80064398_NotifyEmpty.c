#include "../../include/types.h"

extern void *g_main_8012A574_Player;         /* guess: player 1 character object */
extern void *g_main_8012AAD0_Player;         /* guess: player 2 character object */
extern void **g_main_8011EEFC_ObjArray;      /* guess: array of object pointers, indexed by weapon id here */
extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */

/* --- libc-style externs (not yet decompiled) --- */
extern s32 f_main_8008D21C_PlaySound(s32 category, s32 id, void *ctx, void *posArg);

/* Note: the 2nd parameter is overwritten with the 3rd before ever being
   read (the asm moves a2 into a1 at entry) - it's genuinely unused/dead in
   this function; kept only so the signature matches existing call sites
   (e.g. f_main_800644AC_HandleAmmo passes 0x55 there). */
void f_main_80064398_NotifyEmpty(s16 weaponId, s32 unused, s32 skipIfSet) {
    AmmoUser *owner = (AmmoUser *) g_main_8011EEFC_ObjArray[weaponId];
    s32 slot;

    (void) unused;

    if (owner == (AmmoUser *) g_main_8012A574_Player ||
        owner == (AmmoUser *) g_main_8012AAD0_Player) {
        slot = *(s32 *) ((u8 *) owner->hud + 0xCC);
    } else {
        slot = g_main_8011EEF8_WeaponDefs[owner->weaponId].ammoType & 0x3F;
    }

    if (slot == 8) {
        if (skipIfSet != 0)
            return;
        f_main_8008D21C_PlaySound(1, 6, owner, 0);
    } else {
        f_main_8008D21C_PlaySound(1, 0x13, owner, 0);
    }
}
