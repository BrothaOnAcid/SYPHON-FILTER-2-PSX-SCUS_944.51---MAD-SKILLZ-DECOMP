#include "../../include/types.h"

extern s16 D_8012B02C;                 /* not yet decompiled: index into the D_8010C874/D_80168770 tables below */
extern s32 D_8010C874[];               /* not yet decompiled */
extern s32 D_80168770[];               /* not yet decompiled */
extern void *D_8011F6E0;               /* not yet decompiled */
extern u8 D_8012B114[];                /* not yet decompiled */
extern u8 D_80158AD0[];                /* not yet decompiled: string literal */
extern u8 D_80158AE0[];                /* not yet decompiled: string literal */
extern u8 D_8014B950[];                /* not yet decompiled: string literal */

extern void func_800F045C(void *dst, u8 *fmt, s32 a, s32 b); /* not yet decompiled */
extern void func_801591A0(void);                             /* not yet decompiled */
void f_init_8015B794_RunTokenScriptInline(void);
extern void f_init_8015B920_NotifyLevelDelay(void);
extern u32 f_main_8002B0D0_LoadFileAlloc(s8 *name, void **outPtr, s32 size);
extern void func_8002B4C4(void *a, u8 *fmt, s32 c);            /* not yet decompiled */
extern void func_80057DF8(s32 ticks);                           /* not yet decompiled: delay/wait */

/* guess: setup routine run around a stage/menu transition. Formats two
   strings via func_800F045C using entries picked by D_8012B02C from the
   D_8010C874/D_80168770 tables, calls func_801591A0, points
   D_8011F6E0 at D_8012B114, and - only if the low byte of `flag` is set -
   runs f_main_8002B0D0_LoadFileAlloc against it with a 0x1000 size. Finishes by calling
   f_init_8015B794_RunTokenScriptInline, another func_800F045C,
   func_8002B4C4, f_init_8015B920_NotifyLevelDelay and a 0x3C-tick delay via
   func_80057DF8. */
void f_init_8015B96C_LoadWithFlag(s32 flag) {
    u8 sp10[0x40];
    u8 sp30[0x18];

    func_800F045C(sp10, D_80158AD0, D_8010C874[D_8012B02C], D_8010C874[D_8012B02C]);
    func_801591A0();
    D_8011F6E0 = D_8012B114;
    if (flag & 0xFF) {
        f_main_8002B0D0_LoadFileAlloc((s8 *) sp10, &D_8011F6E0, 0x1000);
    }
    f_init_8015B794_RunTokenScriptInline();
    func_800F045C(sp30, D_80158AE0, D_80168770[D_8012B02C], D_8010C874[D_8012B02C]);
    func_8002B4C4(sp30, D_8014B950, 1);
    f_init_8015B920_NotifyLevelDelay();
    func_80057DF8(0x3C);
}
