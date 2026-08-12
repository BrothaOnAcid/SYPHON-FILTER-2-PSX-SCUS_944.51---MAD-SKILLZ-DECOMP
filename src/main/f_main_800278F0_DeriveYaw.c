#include "../../include/types.h"

/* Guessed: 2-arg fixed-point math helper (libc-style, no asm entry). Same
   callee as f_main_8002789C_DeriveRot. */
extern s32 func_800F02AC(s32 a0, s32 a1);

/* Sibling of f_main_8002789C_DeriveRot: derives a heading from `vec`'s x/z
   components directly (vec[0], vec[2]) instead of x/z magnitude, and does
   not negate the result. Only out[0] is written. Returns 0 (ignored by
   callers). */
s32 f_main_800278F0_DeriveYaw(s32 *vec, s32 *out) {
    out[0] = func_800F02AC(vec[0], vec[2]);
    return 0;
}
