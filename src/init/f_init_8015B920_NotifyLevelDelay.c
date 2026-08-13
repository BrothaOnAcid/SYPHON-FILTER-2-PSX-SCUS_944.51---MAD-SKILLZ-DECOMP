#include "../../include/types.h"

extern void *D_8011F6E0;             /* not yet decompiled: set by f_init_8015B96C_LoadWithFlag */
extern s16 D_8011F6DC;               /* not yet decompiled */

extern void func_800177F4(s16 a, s16 b);        /* not yet decompiled */
extern void func_80017804(s32 delay);           /* not yet decompiled */
extern s32 f_init_8015B8C4_GetLevelDelay(void);

/* guess: pushes the (x, y) pair at +0x4E/+0x50 of the struct D_8011F6E0
   points to through func_800177F4, then stashes the level's fixed delay
   (f_init_8015B8C4_GetLevelDelay) into D_8011F6DC and forwards it to
   func_80017804. */
void f_init_8015B920_NotifyLevelDelay(void) {
    u8 *p = (u8 *) D_8011F6E0;
    s16 x = *(s16 *) (p + 0x4E);
    s16 y = *(s16 *) (p + 0x50);

    func_800177F4(x, y);
    D_8011F6DC = (s16) f_init_8015B8C4_GetLevelDelay();
    func_80017804(D_8011F6DC);
}
