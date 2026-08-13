#include "../../include/types.h"

/* guess: third function splat merged into the "func_80161EF8, 0x2DC"
   block (see f_init_80161EF8_SyncWeaponChain / f_init_80161F80_InitWeaponHolder2);
   this one's real prologue starts at 0x801621C0. Clears bit 0x27 on
   `target` and clears bits 0x2C (0x24 & ~0xD3) of its +0x24 flag byte. */
void f_init_801621C0_ClearWeaponFlag2(void *target) {
    u8 *t = (u8 *) target;

    *(t + 0x27) = 0;
    *(t + 0x24) &= 0xD3;
}
