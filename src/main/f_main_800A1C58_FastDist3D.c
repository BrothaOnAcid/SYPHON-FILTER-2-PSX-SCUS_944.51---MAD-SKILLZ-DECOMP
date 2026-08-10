#include "../../include/types.h"

/* Byte-identical duplicate of f_main_8009BFB8_FastDist3D (alpha-max-plus-
   beta-min 3D distance approximation), linked separately - see that file
   for details. */
s32 f_main_800A1C58_FastDist3D(Vec4 *a, Vec4 *b) {
    s32 dx, dy, dz, tmp;

    dx = a->x - b->x;
    if (dx < 0)
        dx = -dx;

    dy = a->y - b->y;
    if (dy < 0)
        dy = -dy;

    dz = a->z - b->z;
    if (dz < 0)
        dz = -dz;

    if (dx < dy) {
        tmp = dx;
        dx = dy;
        dy = tmp;
    }

    if (dx < dz) {
        tmp = dx;
        dx = dz;
        dz = tmp;
    }

    return dx + ((dy + dz) >> 2);
}
