#include "../../include/types.h"

extern ViewportRect *g_main_8011F6E0_Viewport;
extern s16 g_main_8011F6DC_LevelParam;

/* --- libc-style externs (not yet decompiled) --- */
extern void func_800177F4(s16 a0, s16 a1);
extern void func_80017804(s32 a0);

/* Companion to f_init_8015B920_ApplyLevelParam: re-primes with the
   viewport's +0x4E/+0x50 fields, then re-applies the already-cached
   g_main_8011F6DC_LevelParam instead of re-fetching it. */
void f_main_800418E0_ReapplyLevelParam(void) {
    func_800177F4(g_main_8011F6E0_Viewport->unk4E, g_main_8011F6E0_Viewport->unk50);
    func_80017804(g_main_8011F6DC_LevelParam);
}
