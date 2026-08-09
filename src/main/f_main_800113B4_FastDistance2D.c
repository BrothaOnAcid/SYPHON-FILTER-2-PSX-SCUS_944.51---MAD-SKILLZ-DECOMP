#include "../../include/types.h"

/* Fast 2D distance approximation: dx/dy from the +0x0/+0x8 s32 fields of
   each point, sum = |dx| + |dy|, diff = ||dx| - |dy||, with a piecewise
   correction against sum/2 and sum/4 to approximate sqrt(dx^2+dy^2)
   (a variant of the classic "alpha max plus beta min" estimator). */
s32 f_main_800113B4_FastDistance2D(void *arg0, void *arg1) {
    s32 dx = *(s32 *) ((u8 *) arg0 + 0x0) - *(s32 *) ((u8 *) arg1 + 0x0);
    s32 dy = *(s32 *) ((u8 *) arg0 + 0x8) - *(s32 *) ((u8 *) arg1 + 0x8);
    s32 sum, diff, half, quarter;

    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;

    sum = dx + dy;
    diff = dx - dy;
    if (diff < 0)
        diff = -diff;

    half = sum >> 1;
    quarter = sum >> 2;

    if (diff < half) {
        sum -= quarter;
    } else if (half + quarter >= diff) {
        sum -= sum >> 3;
    }

    return sum;
}
