#include "../../include/types.h"

extern s32 g_main_8011EC84_unk;

/* Same packed-value decode as f_main_8001C7F8_DecodeParam (low 16 bits =
   base level, high 16 bits = shift), applied to g_main_8011EC84_unk instead
   of a passed-in param, then scaled by 1.25 (x + x>>2) while the base level
   hasn't saturated to 0x1000; once saturated returns the fixed cap 0x3A98.
   Callers compare the result against a distance - looks like a
   distance/attenuation curve, consistent with g_main_8011EC84_unk's other
   use in f_main_8001C828_ProcessSoundSources. */
s32 f_main_800178EC_RampValue(void) {
    u32 packed = (u32) g_main_8011EC84_unk;
    u32 lo = packed & 0xFFFF;
    s32 shift = (packed >> 16) - 1;
    u32 next = lo + ((0x1000 - lo) >> shift);

    if (lo < 0x1000) {
        return next + (next >> 2);
    }
    return 0x3A98;
}
