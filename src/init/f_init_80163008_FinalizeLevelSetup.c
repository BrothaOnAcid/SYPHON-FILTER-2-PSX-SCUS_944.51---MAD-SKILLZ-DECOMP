#include "../../include/types.h"

/* No glabel of its own (splat folds it into func_80162E44's asm range), but
   it has a clean self-contained prologue/epilogue and is reached via a
   real `jal` (see f_init_80162E44_LoadTypeSlots's tail), so it's kept as
   its own function. */

extern u8 D_8011F624;
extern s32 D_8012A574;
extern s32 D_8012AAD0;
extern s32 D_8011F61C;
extern s32 D_8011F63C;

extern u8 func_800AE2D4(void);        /* not yet decompiled */
extern void func_80040CBC(s32 arg0);  /* not yet decompiled */
extern void func_800AD9F4(void);      /* not yet decompiled */
extern void func_800ACF74(void);      /* not yet decompiled */
extern void func_800AD35C(void);      /* not yet decompiled */
extern void f_init_8015DCE8_MatchWeaponRecords(void);
extern void func_8002AC88(s32 a0, s32 a1, s16 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);

/* If func_800AE2D4() is true, or D_8011F624 is set, runs func_80040CBC on
   D_8012A574 (and, if D_8011F624 is also set, again on D_8012AAD0).
   Otherwise, if D_8011F61C is set, calls func_800AD9F4(); else calls both
   func_800ACF74() and func_800AD35C(). Either way finishes by clearing
   D_8011F63C, running f_init_8015DCE8_MatchWeaponRecords, and dispatching
   func_8002AC88(0xF, 0, -2, -2, 0, 0, 0, 0). */
void f_init_80163008_FinalizeLevelSetup(void) {
    if (func_800AE2D4() != 0 || D_8011F624 != 0) {
        func_80040CBC(D_8012A574);
        if (D_8011F624 != 0) {
            func_80040CBC(D_8012AAD0);
        }
    } else if (D_8011F61C != 0) {
        func_800AD9F4();
    } else {
        func_800ACF74();
        func_800AD35C();
    }

    D_8011F63C = 0;
    f_init_8015DCE8_MatchWeaponRecords();
    func_8002AC88(0xF, 0, -2, -2, 0, 0, 0, 0);
}
