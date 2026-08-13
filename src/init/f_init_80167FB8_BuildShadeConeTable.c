#include "../../include/types.h"

extern u16 D_80169798[0x20][0x10]; /* 32x16 packed-color table (BGR555 + STP bit) */

extern s32 func_80022A18(VECTOR *diff, s32 *out); /* not yet decompiled: PSYQ fixed-point vector length */

/* Builds a 32x16 table of packed 16-bit PS1 colors (grayscale channels +
   semi-transparency bit set) from a fixed-point falloff computed per cell:
   for row `row` (y = -0xF00 + row*0x100) and column `col` (x = (col-7)<<9),
   takes the vector length of (x, y, 0) via func_80022A18, derives a
   fixed-point ratio `a1` from (0x1000 - length), scales it by
   (0x1000 - max(|x|,|y|)/2), rounds, clamps to [0,31], and packs it as a
   grayscale BGR555 color with the STP bit (0x8000) set. Looks like a
   precomputed radial shading/falloff ramp (spot/cone light or similar). */
void f_init_80167FB8_BuildShadeConeTable(void) {
    s32 row;
    s32 y = -0xF00;

    for (row = 0; row < 0x20; row++, y += 0x100) {
        s32 col;

        for (col = 0; col < 0x10; col++) {
            VECTOR v;
            s32 len;
            s32 ratio;
            s32 maxAbs;
            s32 scaled;
            s32 shade;
            u16 packed;

            v.vx = (col - 7) << 9;
            v.vy = y;
            v.vz = 0;

            func_80022A18(&v, &len);

            {
                s32 t = (0x1000 - len) * 5 * 31;
                ratio = (t >= 0) ? (t >> 13) : ((t + 0x1FFF) >> 13);
            }

            {
                s32 ax = v.vx < 0 ? -v.vx : v.vx;
                s32 ay = v.vy < 0 ? -v.vy : v.vy;
                maxAbs = (ax < ay) ? ay : ax;
            }

            scaled = 0x1000 - (maxAbs >> 1);

            {
                s32 t = ratio * scaled;
                shade = (t >= 0) ? (t >> 12) : ((t + 0xFFF) >> 12);
            }

            if (shade < 0) {
                shade = 0;
            }
            if (shade > 0x1F) {
                shade = 0x1F;
            }

            packed = (u16) ((shade << 10) | (shade << 5) | shade | 0x8000);
            D_80169798[row][col] = packed;
        }
    }
}
