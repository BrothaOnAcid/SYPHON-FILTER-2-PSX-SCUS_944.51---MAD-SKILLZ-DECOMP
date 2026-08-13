#include "../../include/types.h"

extern void f_init_8015BB50_DispatchWeaponSound(s16 weaponId, s32 arg1);
extern void func_801631C0(void *target, s32 flag); /* not yet decompiled */

/* guess: fifth (final) function splat merged into the "func_80160BA4,
   0x2C0" block (see knowledge.txt); this one's real prologue starts at
   0x80160DD4. Looks like a companion "reset" to
   f_init_80160BF4_InitWeaponHolder: resets the weapon's sound state,
   clears the +0x26 flag, forces the linked node at +0x18's +0x8 halfword
   to 0x7FFF and the inner (+0x8) object's +0x9 byte to 0x40, clears bit
   3 (0x28) of +0x0, runs func_801631C0, then sets or clears bit 7 (0x80)
   of +0x1 depending on whether +0xC is set. */
void f_init_80160DD4_ResetWeaponHolder(void *target) {
    u8 *t = (u8 *) target;
    s16 weaponId = *(s16 *) (t + 2);

    f_init_8015BB50_DispatchWeaponSound(weaponId, 0);
    *(t + 0x26) = 0;

    *(s16 *) (*(u8 **) (t + 0x18) + 8) = 0x7FFF;
    *(*(u8 **) (t + 8) + 9) = 0x40;

    *t &= 0xD7;
    func_801631C0(target, 1);

    if (*(s32 *) (t + 0xC) != 0) {
        *(t + 1) |= 0x80;
    } else {
        *(t + 1) &= 0x7F;
    }
}
