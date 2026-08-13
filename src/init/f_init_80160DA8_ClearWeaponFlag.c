#include "../../include/types.h"

extern s16 D_8011F648; /* not yet decompiled, see f_init_8015E0D0_HandleLevelEvent */

/* guess: third function splat merged into the "func_80160BA4, 0x2C0"
   block (see knowledge.txt); this one's real prologue starts at
   0x80160DA8. Stashes `val` into D_8011F648, clears bit 0x27 on `target`
   and clears bit 1 (0x02) of its +0x24 flag byte. */
void f_init_80160DA8_ClearWeaponFlag(void *target, s16 val) {
    u8 *t = (u8 *) target;

    D_8011F648 = val;
    *(t + 0x27) = 0;
    *(t + 0x24) &= 0xFD;
}
