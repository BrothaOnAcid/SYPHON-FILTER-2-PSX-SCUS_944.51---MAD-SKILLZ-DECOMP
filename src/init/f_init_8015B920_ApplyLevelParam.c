#include "../../include/types.h"

extern ViewportRect *g_main_8011F6E0_Viewport;
extern s16 g_main_8011F6DC_LevelParam;

/* --- libc-style externs (not yet decompiled) --- */
extern void func_800177F4(s16 a0, s16 a1);
extern void func_80017804(s32 a0);
extern s32 f_init_8015B8C4_GetLevelParam(void);

/* Glue: primes something with the viewport's +0x4E/+0x50 fields, fetches
   the per-level constant, caches it (g_main_8011F6DC_LevelParam), and
   applies it via func_80017804. */
void f_init_8015B920_ApplyLevelParam(void) {
    s16 param;

    func_800177F4(g_main_8011F6E0_Viewport->unk4E, g_main_8011F6E0_Viewport->unk50);

    param = (s16) f_init_8015B8C4_GetLevelParam();
    g_main_8011F6DC_LevelParam = param;
    func_80017804(param);
}
