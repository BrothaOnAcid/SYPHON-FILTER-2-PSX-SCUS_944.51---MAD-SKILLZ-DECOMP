#include "../../include/types.h"

/* Fixed-point (1.0 = 0x1000) sin/cos table, indexed by a 12-bit angle. Each
   entry packs (cos16 << 16) | sin16. */
extern u32 g_main_80117D98_SinCosLut[];

/* Resolves one HAN 12-bit signed angle `w` into fixed-point cos/sin (sign is
   folded into sin; cos is always positive). */
static void dec_angle(s16 w, s32 *cos, s32 *sin) {
    u32 e = g_main_80117D98_SinCosLut[(w < 0 ? -w : w) & 0xFFF];
    *cos = (s32) (s16) (e >> 16);
    *sin = (s32) (s16) (e & 0xFFFF);
    if (w < 0)
        *sin = -*sin;
}

/* Builds the 3x3 s16 rotation matrix at `m` from 3 HAN 12-bit angles using
   fixed-point trig, then returns `m`. Called by f_main_8001C4F4_ReadKeyframes
   (which ignores the return value). */
s16 *f_main_8001C264_BuildMatrix(const s16 *angles, s16 *m) {
    s32 cA, sA, cB, sB, cC, sC;
    s32 r;
    s32 s;

    dec_angle(angles[0], &cA, &sA);
    dec_angle(angles[1], &cB, &sB);
    dec_angle(angles[2], &cC, &sC);

    r = FIX12(sB, sA);
    s = FIX12(cB, sA);

    m[0] = (s16) (FIX12(cB, cC) + FIX12(r, sC));
    m[1] = (s16) (-FIX12(cB, sC) + FIX12(r, cC));
    m[2] = (s16) FIX12(sB, cA);
    m[3] = (s16) FIX12(sC, cA);
    m[4] = (s16) FIX12(cC, cA);
    m[5] = (s16) -sA;
    m[6] = (s16) (-FIX12(sB, cC) + FIX12(s, sC));
    m[7] = (s16) (FIX12(sB, sC) + FIX12(s, cC));
    m[8] = (s16) FIX12(cB, cA);

    return m;
}
