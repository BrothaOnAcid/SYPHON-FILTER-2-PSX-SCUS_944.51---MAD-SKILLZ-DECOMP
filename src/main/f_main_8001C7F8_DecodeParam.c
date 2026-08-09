#include "../../include/types.h"

/* Decodes a packed 32-bit value (high 16 bits = shift/exponent-ish factor,
   low 16 bits = base) into a curve value in the same style as the fixed
   0x1000/0x2000 constants seen throughout f_main_8001C828_ProcessSoundSources -
   most likely a distance/attenuation lookup. Returns 0x2000 (max?) when the
   packed value is 0 (no entry). */
s32 f_main_8001C7F8_DecodeParam(s32 packed) {
    s32 lo = packed & 0xFFFF;

    if (packed != 0) {
        return lo + ((0x1000 - lo) >> ((packed >> 16) - 1));
    }
    return 0x2000;
}
