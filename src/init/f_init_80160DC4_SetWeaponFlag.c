#include "../../include/types.h"

extern s16 D_8011F8AC; /* not yet decompiled */

/* guess: fourth function splat merged into the "func_80160BA4, 0x2C0"
   block (see knowledge.txt); this one's real prologue starts at
   0x80160DC4. Stashes `val` into D_8011F8AC and clears bit 0x27 on
   `target` (same field f_init_80160DA8_ClearWeaponFlag clears). */
void f_init_80160DC4_SetWeaponFlag(void *target, s16 val) {
    u8 *t = (u8 *) target;

    D_8011F8AC = val;
    *(t + 0x27) = 0;
}
