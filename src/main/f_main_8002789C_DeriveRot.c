#include "../../include/types.h"

/* Horizontal (xz) magnitude of a vector - sqrt(vec0^2 + vec2^2). */
extern void f_main_800232A8_MagXZ(s32 *vec, s32 *out);

/* Guessed: 2-arg fixed-point math helper (libc-style, no asm entry). */
extern s32 func_800F02AC(s32 a0, s32 a1);

/* Derives a rotation value from `vec`: out[0] = -func_800F02AC(vec[1], mag)
   where mag is the horizontal magnitude of (vec[0], vec[2]). Only out[0] is
   written. Returns 0 (ignored by callers). */
void f_main_8002789C_DeriveRot(s32 *vec, s32 *out) {
    s32 mag;

    f_main_800232A8_MagXZ(&vec[0], &mag);
    out[0] = -func_800F02AC(vec[1], mag);
}
