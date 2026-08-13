#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */

extern void f_init_8015BB50_DispatchWeaponSound(s16 weaponId, s32 arg1);
extern void func_8004CD34(void *target, void *table, s32 a2);          /* not yet decompiled */
extern void func_801631C0(void *target, s32 flag);                      /* not yet decompiled */
extern void func_80066924(void *target, s32 a1, s32 a2, s32 a3, s32 a4); /* not yet decompiled */
extern void func_80039750(void *target, s32 a1, s32 a2);                /* not yet decompiled */
extern s32 func_8008FBA4(void *obj, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void func_8002CED4(void *dst, s32 val);

/* guess: a variant of f_init_80160BF4_InitWeaponHolder for a different
   weapon-holder "kind" - same overall shape (reset sound, run
   func_8004CD34 against `target`'s own +0x14 table pointer instead of a
   fixed one, twiddle flag bits, run func_801631C0/func_80066924/
   func_80039750/func_8008FBA4), but ends by zeroing
   g_main_8011EEF8_WeaponDefs[weaponId].unk30 and clearing part of
   `target`'s +0x4 flags instead of allocating/linking a node. `flags`
   bit 0 forces the linked node's (+0x18) +0x8 halfword to the "unset"
   sentinel 0x7FFF; bit 2 selects between two 2-bit codes packed into
   +0x4 bits 16-17. */
void f_init_80161F80_InitWeaponHolder2(void *target, s32 flags) {
    u8 *t = (u8 *) target;
    s16 weaponId = *(s16 *) (t + 2);
    WeaponDef *def;

    f_init_8015BB50_DispatchWeaponSound(weaponId, 0);

    *(t + 0x26) = 7;
    func_8004CD34(target, *(void **) (t + 0x14), 3);
    *(t + 0x25) = 0xAA;

    if (flags & 1) {
        *(s16 *) (*(u8 **) (t + 0x18) + 8) = 0x7FFF;
    }

    *(s32 *) (t + 0x10) = 0;
    *t = (u8) (*t & 0x95);
    *(t + 1) = (u8) (*(t + 1) & 0x80);
    *(t + 0x24) |= 0x80;
    *(s32 *) (t + 4) &= ~0xF00;

    *t &= 0xEF;
    *(s32 *) (t + 4) = (*(s32 *) (t + 4) & 0xFF00FFFF) | ((flags & 4) ? 0x30000 : 0);
    func_801631C0(target, 1);

    if (*(s32 *) (t + 0xC) != 0) {
        *(t + 1) |= 0x80;
    } else {
        *(t + 1) &= 0x7F;
        func_80066924(target, 0, 0, 0, 0);
    }

    func_80039750(target, 0xA000, 1);
    func_8008FBA4(target, *(s32 *) (*(u8 **) (t + 0xC) + 4), 0x406, 0x800, -1, 0xC00);

    def = &g_main_8011EEF8_WeaponDefs[weaponId];

    if ((*(t + 1) & 0x80) == 0 && (def->unk4A & 2) != 0) {
        *(*(u8 **) (t + 8) + 8) |= 8;
    } else {
        *(*(u8 **) (t + 8) + 0xB) |= 0x40;
    }

    def->unk30 = 0;

    func_8002CED4(*(u8 **) (t + 8) + 0xC, 0);
    *(s32 *) (t + 4) &= 0xFFFF0FFF;
}
