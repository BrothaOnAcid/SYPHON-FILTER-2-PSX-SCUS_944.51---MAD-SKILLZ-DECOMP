#include "../../include/types.h"

/* The raw PS1 GTE ops below (mtc2/rtps/mfc2) cannot be compiled on a
   non-MIPS host, so they are modeled as these placeholder macros; the real
   data comes from the GTE coprocessor at runtime. */
#define GTE_MTC2(v, reg) ((void) (v))
#define GTE_RTPS()       ((void) 0)
#define GTE_MFC2(reg)    ((u32) 0)

/* Loads (x, y, z) into the GTE, runs RTPS, and returns the projected SZ3
   screen depth. */
s32 f_main_80010810_ProjectDepth(s32 x, s32 y, s32 z) {
    u32 xy;

    xy = ((u32) x & 0xFFFF) | ((u32) (-y) << 16);
    GTE_MTC2(xy, 0);   /* VXY0 = X | (-Y << 16) */
    GTE_MTC2(z, 1);    /* VZ0 = Z */
    GTE_RTPS();        /* project the point */
    return (s32) GTE_MFC2(19); /* SZ3: screen depth */
}
