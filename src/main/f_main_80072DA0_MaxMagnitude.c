#include "../../include/types.h"

/* Guessed: leading-zero-ish bit count / normalization-shift helper (no asm
   entry - stuck in the misclassified data region, see DOC.md). */
extern s32 func_800F115C(s32 val);

/* Returns 0 for a zero input; otherwise right-shifts 0x7FFFFFFF by
   func_800F115C(abs(val)), i.e. the largest signed value representable at
   `val`'s magnitude - a fixed-point normalization range helper. */
u32 f_main_80072DA0_MaxMagnitude(s32 val) {
    if (val == 0) {
        return 0;
    }
    if (val < 0) {
        val = -val;
    }
    return 0x7FFFFFFFU >> func_800F115C(val);
}
