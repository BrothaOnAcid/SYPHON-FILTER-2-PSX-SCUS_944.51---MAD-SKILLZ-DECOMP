#include "../../include/types.h"

/* --- libc/PSYQ-region externs, not yet decompiled (see DOC.md) --- */
extern s32 func_800F64D4(s32 id, s32 arg1, s32 arg2);
extern s32 func_800F6574(s32 id);
extern void func_80167510(Preset4 *from, Preset4 *to, s32 ratio, void *dst, s32 arg4, s32 size);

extern Preset4 g_init_80158BEC_Preset;
extern u8 g_main_8011F04C_Dst[0x960];

/* Snapshots g_init_80158BEC_Preset into two identical stack copies
   (from/to - both taken before any mutation, so this call is effectively
   a self-blend), computes a ratio via func_800F64D4/func_800F6574 (both
   fed a shared id 0x19D), and hands it to func_80167510 to build
   g_main_8011F04C_Dst (0x960 bytes). */
void f_init_80167638_BuildBlend(void) {
    Preset4 from;
    Preset4 to;
    s32 scaled;
    s32 ratio;

    from = g_init_80158BEC_Preset;
    to = g_init_80158BEC_Preset;

    scaled = func_800F64D4(0x19D, g_init_80158BEC_Preset.unk8, g_init_80158BEC_Preset.unkC);
    ratio = (scaled * 0xC0) / func_800F6574(0x19D);

    func_80167510(&from, &to, ratio, g_main_8011F04C_Dst, 0, 0x960);
}
