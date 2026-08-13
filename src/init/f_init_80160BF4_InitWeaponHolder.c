#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */
extern u8 D_8012C7D8[];                        /* not yet decompiled: table/template passed to func_8004CD34 */

extern void f_init_8015BB50_DispatchWeaponSound(s16 weaponId, s32 arg1);
extern void func_8004CD34(void *target, u8 *table, s32 a2);       /* not yet decompiled */
extern void func_8002CED4(void *dst, s32 val);
extern void func_80066924(void *target, s32 a1, s32 a2, s32 a3, s32 a4); /* not yet decompiled */
extern void func_80039750(void *target, s32 a1, s32 a2);           /* not yet decompiled */
extern s32 func_8008FBA4(void *obj, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void *func_80025AD0(s32 size);                               /* not yet decompiled: bump allocator */
extern void f_init_80160BA4_InitWeaponLinkNode(s32 arg0, void *b, s32 c);
extern void func_80017678(void *ctx, s32 val, s32 a2);

/* guess: second function splat merged into the "func_80160BA4, 0x2C0"
   block (see knowledge.txt / f_init_80160BA4_InitWeaponLinkNode); this
   one's real prologue starts at 0x80160BF4. Configures a weapon-holder
   object `target` (mode `kind` selects a melee-ish branch when it's
   0x40): resets the weapon's sound state via
   f_init_8015BB50_DispatchWeaponSound, runs func_8004CD34 against
   D_8012C7D8, sets/clears various flag bits at +0x0/+0x1/+0x4/+0x10 and
   on the object at +0x8, then runs func_80066924/func_80039750/
   func_8008FBA4 subsystem setup calls. Allocates a fresh 0x50-byte link
   node, stamps it into g_main_8011EEF8_WeaponDefs[weaponId]+0x24 (same
   field f_init_8015EE20_AttachWeaponLink writes) via
   f_init_80160BA4_InitWeaponLinkNode, then wires it up with
   func_80017678 and (unless `kind` is 0x40) sets bit 1 on the node's
   +0x4 flags. */
void f_init_80160BF4_InitWeaponHolder(void *target, s32 kind) {
    u8 *t = (u8 *) target;
    s16 weaponId = *(s16 *) (t + 2);
    void *node;

    f_init_8015BB50_DispatchWeaponSound(weaponId, 0);

    *(t + 0x26) = 0xB;
    func_8004CD34(target, D_8012C7D8, 0);

    *(s32 *) (t + 0x10) = 0;

    if (kind == 0x40) {
        u8 *inner = *(u8 **) (t + 8);
        *(inner + 8) |= 8;
        *t |= 0x20;
        *(s32 *) (t + 4) = (*(s32 *) (t + 4) & ~0xFF) | 0x12;
    } else {
        u8 *inner = *(u8 **) (t + 8);
        *(s32 *) (inner + 0x18) = 0;
        *(inner + 9) = 0x17;
        func_8002CED4(*(u8 **) (t + 8) + 0xC, 0);
        *t &= 0xDF;
    }

    *t = (u8) ((*t | 3) & 0xB7);
    *(t + 1) = 0;
    func_80066924(target, 0, 0, 0, 0);

    func_80039750(target, 0x800, 1);
    func_8008FBA4(target, *(s32 *) (*(u8 **) (t + 0xC) + 4), 0x666, 0x1000, 0x800, 0);

    node = func_80025AD0(0x50);
    f_init_80160BA4_InitWeaponLinkNode(0, node, 0xFF);
    /* NOTE: same +0x24 field / ammoType conflict as f_init_8015EE20_AttachWeaponLink */
    *(void **) ((u8 *) g_main_8011EEF8_WeaponDefs + weaponId * 0x4C + 0x24) = node;

    func_80017678(node, *(s32 *) (*(u8 **) (t + 8) + 0xC), 0);

    if (kind != 0x40) {
        *(s32 *) ((u8 *) node + 4) |= 2;
    }
}
