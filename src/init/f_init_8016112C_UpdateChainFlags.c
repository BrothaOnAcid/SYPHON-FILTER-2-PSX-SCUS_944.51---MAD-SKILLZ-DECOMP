#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */
extern SubDefEntry D_8011F59C[];              /* table indexed by WeaponDef.unk00 */

void f_init_8015E7AC_ResolveChainedWeapon(void *arg0);
extern u8 func_800AB584(u8 mode, s16 *out0, s16 *out2); /* not yet decompiled */
extern void func_80017F3C(void *obj);                    /* not yet decompiled */

/* arg0: an ObjRecord. If `cmd` isn't 0x47, defers to
   f_init_8015E7AC_ResolveChainedWeapon(arg0); if it IS 0x47, resets
   arg0->unk4 to -1 directly instead. Either way, then calls
   func_800AB584(arg0->unk1, &a, &b) to get a 2-value status.

   If b == 0: if a is also 0, sets arg0's flags0 bit 0x20, then follows
   WeaponDefs[weaponId]'s SubDefEntry (via unk00) - if its unk4 flag byte
   (dereferenced) is nonzero and arg0->unk8's own +0xA has bit 8 set, calls
   func_80017F3C(arg0->unk8).
   If b != 0: if a is nonzero, clears arg0's flags1 bit 0x40. */
void f_init_8016112C_UpdateChainFlags(ObjRecord *arg0, s32 cmd) {
    s16 a, b;

    if (cmd != 0x47) {
        f_init_8015E7AC_ResolveChainedWeapon(arg0);
    } else {
        arg0->unk4 = (u32) -1;
    }

    func_800AB584(arg0->unk1, &a, &b);

    if (b == 0) {
        if (a == 0) {
            SubDefEntry *sub;

            arg0->flags0 |= 0x20;
            sub = &D_8011F59C[g_main_8011EEF8_WeaponDefs[arg0->weaponId].unk00];
            if (*sub->unk4 != 0) {
                void *obj = arg0->unk8;
                if (*((u8 *) obj + 0xA) & 8) {
                    func_80017F3C(obj);
                }
            }
        }
    } else if (a != 0) {
        arg0->unk1 &= 0xBF;
    }
}
