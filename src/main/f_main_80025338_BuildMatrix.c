#include "../../include/types.h"

/* Guessed: takes a 9-short grid (3 rows x 3 cols, row = axis x/y/z, col =
   which source vector) - purpose unconfirmed, possibly a short-vector
   matrix/basis construction for the GTE. Not yet decompiled. */
extern s16 func_800249F4(s16 *grid);

/* Interleaves 3 VECTORs (a, b, and a 4th passed via the stack, `d`) into a
   9-short grid - row i holds {a[i], b[i], d[i]} for i = x,y,z - and passes
   it to func_800249F4. */
s16 f_main_80025338_BuildMatrix(void *unused0, void *unused1, VECTOR *a, VECTOR *b, VECTOR *d) {
    s16 grid[9];

    grid[0] = (s16) a->vx;
    grid[1] = (s16) b->vx;
    grid[2] = (s16) d->vx;
    grid[3] = (s16) a->vy;
    grid[4] = (s16) b->vy;
    grid[5] = (s16) d->vy;
    grid[6] = (s16) a->vz;
    grid[7] = (s16) b->vz;
    grid[8] = (s16) d->vz;

    return func_800249F4(grid);
}
