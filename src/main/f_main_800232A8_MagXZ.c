#include "../../include/types.h"

/* Fixed-point multiply: (a0 * a1) >> 12. */
extern s32 f_main_80010654_FixedMul(s32 a0, s32 a1);

/* Guessed: fixed-point square root (libc-style, no asm entry). */
extern s32 func_800F6434(s32 a0);

/* Computes the horizontal (xz) magnitude of `vec` from its components 0 and
   2: sqrt(vec0^2 + vec2^2) in 12-bit fixed point, stored at out[0]. Returns
   0 (ignored by callers). Used by f_main_8002789C_DeriveRot. */
void f_main_800232A8_MagXZ(s32 *vec, s32 *out) {
    out[0] = func_800F6434(f_main_80010654_FixedMul(vec[0], vec[0]) +
                           f_main_80010654_FixedMul(vec[2], vec[2]));
}
