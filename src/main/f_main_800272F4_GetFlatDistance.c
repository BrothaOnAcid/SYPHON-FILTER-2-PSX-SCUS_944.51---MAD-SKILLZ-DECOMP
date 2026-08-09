#include "../../include/types.h"

/* Suspicious: likely a PSYQ library routine (fixed-point vector length /
   square-root helper, uses the bignum sqrt lookup table D_80108900). */
extern s32 func_80022A18(VECTOR *diff, void *out);

s32 f_main_800272F4_GetFlatDistance(VECTOR *a, VECTOR *b, void *out) {
    VECTOR d;

    d.vx = a->vx - b->vx;
    d.vy = a->vy - b->vy;
    d.vz = a->vz - b->vz;
    d.vy = 0;

    func_80022A18(&d, out);

    return 0;
}
