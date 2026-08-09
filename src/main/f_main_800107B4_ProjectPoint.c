#include "../../include/types.h"

/* The raw PS1 GTE ops below (mtc2/lwc2/rtps/mfc2) cannot be compiled on a
   non-MIPS host, so they are modeled as these placeholder macros; the real
   data comes from the GTE coprocessor at runtime. */
#define GTE_MTC2(v, reg) ((void) (v))
#define GTE_LWC2(v, reg) ((void) (v))
#define GTE_RTPS()       ((void) 0)
#define GTE_MFC2(reg)    ((u32) 0)

/* Guessed input point: X/Y as 32-bit values packed into VXY0 (X low 16, -Y
   high 16), Z loaded raw into VZ0. */
typedef struct {
    s32 x;      /* +0x00 */
    s32 y;      /* +0x04 */
    s32 z;      /* +0x08 */
} ProjInPoint;

/* Guessed output: projected screen X/Y (Y sign-flipped back) and raw SZ3
   depth. */
typedef struct {
    s32 x;      /* +0x00 */
    s32 y;      /* +0x04 */
    s32 sz;     /* +0x08 */
} ProjOutPoint;

/* Loads a point's X/Y/Z into the GTE, runs RTPS, and writes the projected
   screen X/Y (Y re-negated) plus raw SZ3 depth to *out. */
void f_main_800107B4_ProjectPoint(ProjInPoint *in, ProjOutPoint *out) {
    u32 xy;
    s32 sxy;
    s32 sz;

    xy = ((u32) in->x & 0xFFFF) | ((u32) (-in->y) << 16);
    GTE_MTC2(xy, 0);          /* VXY0 = X | (-Y << 16) */
    GTE_LWC2(in->z, 1);       /* VZ0 = Z */
    GTE_RTPS();               /* project the point */

    sxy = (s32) GTE_MFC2(14); /* SXY2: screen X (low16) / Y (high16) */
    sz = (s32) GTE_MFC2(19);  /* SZ3: screen depth */

    out->sz = sz;
    out->y = -(sxy >> 16);
    out->x = (s32) ((s16) sxy);
}
