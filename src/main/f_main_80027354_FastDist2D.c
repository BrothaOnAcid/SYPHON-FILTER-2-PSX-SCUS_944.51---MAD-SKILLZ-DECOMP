#include "../../include/types.h"

/* Fast X/Z-plane distance approximation (alpha-max-plus-beta-min style):
   given two Vec4 positions (y/w unused), computes |dx|+|dz| then refines it
   toward a closer octagonal-distance estimate and writes the result to
   *out. Always returns 0. */
s32 f_main_80027354_FastDist2D(Vec4 *a, Vec4 *b, s32 *out) {
    s32 dx, dz, sum, diff, hi, lo;

    dx = a->x - b->x;
    if (dx < 0)
        dx = -dx;

    dz = a->z - b->z;
    if (dz < 0)
        dz = -dz;

    sum = dx + dz;
    hi = sum >> 1;
    lo = sum >> 2;

    diff = dx - dz;
    if (diff < 0)
        diff = -diff;

    if (diff < hi)
        sum -= lo;
    else if ((hi + lo) >= diff)
        sum -= sum >> 3;

    *out = sum;
    return 0;
}
