#include "../../include/types.h"

/* Weapon-holder objects that take the "ammo spend" path (block B below);
   everything else goes through block A (weapon/ammo-type resolution). */
extern void *g_main_8012A574_Player;   /* guess: player 1 character object */
extern void *g_main_8012AAD0_Player;   /* guess: player 2 character object */
extern WeaponDef *g_main_8011EEF8_WeaponDefs;  /* weapon definitions, indexed by weapon id */
extern AmmoDef g_main_8012F630_AmmoDefs[];     /* ammo-type definitions, 0x24-byte records */
extern u8 g_main_8011F624_Flag;        /* guess: debug/cheat "don't spend ammo" flag */

/* --- libc-style externs (not yet decompiled) --- */
extern void func_80042CF4(s16 a0, s32 a1);
extern void func_8005CEC4(s32 a0);
extern s32 func_800642F4(AmmoUser *a0, s32 a1);
extern void f_main_80064398_NotifyEmpty(s16 weaponId, s32 unused, s32 skipIfSet);  /* decompiled */
extern void func_80064464(s16 a0, s32 a1);
extern void func_80078AC8(s32 a0, s16 a1, s32 a2, s16 a3);
extern s32 f_main_8008D21C_PlaySound(s32 category, s32 id, void *ctx, void *posArg);  /* decompiled */
extern void func_8008E370(s32 a0, s8 a1, void *a2, s32 a3);
extern s8 func_8014C8A4(s32 a0, s8 a1);

/* Ammo/weapon handler.
   Block A (weapon select, non-player objects): resolves the ammo type from
   the weapon definition table, refills/consumes the per-weapon reserve
   counter, and drives the reload animation.

   Block B (player characters): spends the current ammo slot (the 0x80064750
   decrement gated by AmmoDef.spend - zeroing the spend byte makes ammo never
   go down), spawns the projectile, and triggers the reload/hud update when
   the current clip runs out. `slot` selects the ammo slot; 0 means "use the
   hud block's +0xCC default slot". */
void f_main_800644AC_HandleAmmo(AmmoUser *user, s32 slot) {
    AmmoDef *def;
    AmmoSlot *as;
    PerWeapon *perW;
    u8 *base;
    u8 *hud;
    s32 spent;
    s32 v;
    u32 bit;
    u32 tmp;
    s8 r;

    if (user != (AmmoUser *) g_main_8012A574_Player &&
        user != (AmmoUser *) g_main_8012AAD0_Player) {
        slot = g_main_8011EEF8_WeaponDefs[user->weaponId].ammoType & 0x3F;
        def = &g_main_8012F630_AmmoDefs[slot];

        if (def->unk07 == -2) {
            if (g_main_8011F624_Flag != 0)
                r = func_8014C8A4(slot, def->unk07);
            else
                r = def->unk06;
            f_main_8008D21C_PlaySound(1, r, user, 0);
        } else if (def->unk07 >= 0) {
            func_8008E370(1, def->unk07, user,
                          (s32) ((u32) (*(s32 *) ((u8 *) user->unk1C + 0xC)) << 20) >> 28);
        }

        func_80064464(user->weaponId, slot);

        if ((def->flags03 & 7) != 0 || (spent = 1, slot == 9)) {
            perW = *(PerWeapon **) ((u8 *) user->unk1C + 8);
            perW->unk3A -= 1;
            spent = 0;
            if (perW->unk3A == 0) {
                spent = 1;
                perW->unk3A = def->reserve;
            }
        }

        if (user->flags != 0 && spent) {
            perW = *(PerWeapon **) ((u8 *) user->unk1C + 8);
            perW->unk30 = (slot == 8 || slot == 0x15 || slot == 0x1C) ? 0x55 : 0x3E;
            perW->unk34 = 0x14;
            if (user->core->act->unk38 == 6)
                func_80042CF4(user->weaponId, 6);
            func_80042CF4(user->weaponId, perW->unk30);
        }
        return;
    }

    hud = user->hud;
    if (slot == 0)
        slot = *(s32 *) (hud + 0xCC);
    as = (AmmoSlot *) (hud + 0x44 + slot * 4);

    if (as->current != 0) {
        def = &g_main_8012F630_AmmoDefs[slot];
        if (def->spend != 0)
            as->current -= 1;          /* 80064750: the ammo spend */

        r = def->unk06;
        if (def->unk06 >= 0) {
            if (g_main_8011F624_Flag != 0)
                r = func_8014C8A4(slot, def->unk06);
            f_main_8008D21C_PlaySound(1, r, user, 0);
            if ((u32) (slot - 0x16) >= 2U) {
                func_80078AC8(2, user->weaponId, 0, g_main_8012F630_AmmoDefs[slot].unk08);
                func_80064464(user->weaponId, slot);
            }
        }
    }

    if (user->flags[0xC] != 8 &&
        ((g_main_8012F630_AmmoDefs[slot].flags03 & 7) == 0 ||
         (as->current == 0 && as->max != 0))) {
        if (slot != 8 && slot != 0x15 && slot != 0x1C) {
            if ((func_800642F4(user, slot) & 0xFF) != 0) {
                if (user->core->act->unk38 == 6)
                    func_80042CF4(user->weaponId, 6);
                func_80042CF4(user->weaponId, 0x3E);
            }
        } else {
            v = (s32) (PTR_U32(user->flags) & 3);
            base = (u8 *) ((u64) (PTR_U32(user->flags) & ~3U));
            bit = (v != 0) ? (u32) v * 8 + 3 : 3;
            tmp = *(u32 *) (base + ((bit >> 5) << 2)) & (1u << (bit & 0x1F));
            if (tmp != 0) {
                if (slot == 8)
                    f_main_80064398_NotifyEmpty(user->weaponId, 0x55, 0);
            } else {
                bit = (v != 0) ? (u32) v * 8 + 2 : 2;
                tmp = *(u32 *) (base + ((bit >> 5) << 2)) & (1u << (bit & 0x1F));
                if (tmp != 0) {
                    if (slot == 8)
                        f_main_80064398_NotifyEmpty(user->weaponId, 0x55, 0);
                } else {
                    if (user->core->act->unk38 == 6)
                        func_80042CF4(user->weaponId, 6);
                    func_80042CF4(user->weaponId, 0x55);
                }
            }
        }
    }

    func_8005CEC4(*(s32 *) (hud + 0xD8));
}
