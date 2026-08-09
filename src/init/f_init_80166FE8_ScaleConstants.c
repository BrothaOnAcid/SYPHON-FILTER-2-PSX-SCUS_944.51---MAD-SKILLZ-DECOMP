#include "../../include/types.h"

extern s32 g_init_80116BB0_Constants[4];
extern s32 g_init_8013C9B0_Scaled[4];

extern s32 f_main_80010654_FixedMul(s32 a0, s32 a1);

/* Scales 4 fixed-point constants by `factor` (12-bit fixed-point multiply)
   into g_init_8013C9B0_Scaled. */
void f_init_80166FE8_ScaleConstants(s32 factor) {
    s32 i;

    for (i = 0; i < 4; i++)
        g_init_8013C9B0_Scaled[i] = f_main_80010654_FixedMul(g_init_80116BB0_Constants[i], factor);
}
