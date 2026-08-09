#include "../../include/types.h"

/* The raw PS1 GTE ops below (mtc2/mfc2/rtps/ctc2) cannot be compiled on a
   non-MIPS host, so they are modeled as these placeholder macros; the real
   data comes from the GTE coprocessor at runtime. */
#define GTE_MTC2(v, reg) ((void) (v))
#define GTE_CTC2(v, reg) ((void) (v))
#define GTE_RTPS()       ((void) 0)
#define GTE_MFC2(reg)    ((u32) 0)

/* Projects the GTE local vector V0 (which the caller must have preloaded) to
   screen space: zeroes matrix rows R11/R12 (data reg 0) and R13/R21 (data reg
   1) of the current camera matrix, then runs an RTPS.
   Returns (3 * SZ0) >> 2 when 0 < SZ0 <= near and |IR2| < zmax, else 0; the
   value doubles as a visibility flag (0 = culled). SZ0 and IR2 are the RTPS
   screen-depth and the MAC2/2 read-backs. */
s32 f_main_8001AA5C_ProjCheck(s32 near, s32 zmax) {
    s32 sz;
    s32 d;

    GTE_MTC2(0, 0);              /* mtc2 $zero, $0: R11/R12 = 0 */
    GTE_MTC2(0, 1);              /* mtc2 $zero, $1: R13/R21 = 0 */
    GTE_RTPS();                  /* rtps: project V0 */
    sz = (s32) GTE_MFC2(19);     /* SZ0: screen depth */
    if (sz <= 0)
        return 0;
    if (sz - near > 0)
        return 0;
    d = (s32) GTE_MFC2(14);      /* IR2: rotated coordinate / 2 */
    if (d < 0)
        d = -d;
    if (d - zmax >= 0)
        return 0;
    return (3 * sz) >> 2;
}
