#include "../../include/types.h"

/* Fixed-point multiply with 12 fractional bits: (a0 * a1) >> 12, using the
   full signed 64-bit product (mult/mfhi/mflo). Only the low 32 bits of the
   shifted product are kept. The negative-product branch reproduces the asm's
   arithmetic-shift idiom exactly; real callers square components (always
   non-negative products), so it is never hit in practice. */
s32 f_main_80010654_FixedMul(s32 a0, s32 a1) {
    s32 hi;
    u32 lo;
    s32 r;

    hi = (s32) (((s64) a0 * (s64) a1) >> 32);
    lo = (u32) ((s64) a0 * (s64) a1);
    if (hi >= 0) {
        r = (s32) (((u32) hi << 20) + (lo >> 12));
    } else {
        lo = (u32) -(s32) lo;
        if (lo == 0)
            hi = -hi;
        r = (s32) (((u32) hi << 20) + (lo >> 12));
        r = -r;
    }
    return r;
}
