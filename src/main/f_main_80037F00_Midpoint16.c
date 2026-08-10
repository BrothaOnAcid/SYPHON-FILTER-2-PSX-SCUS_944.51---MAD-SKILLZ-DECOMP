#include "../../include/types.h"

/* Guessed packed-s16 3-component point (x/y/z at +0/+2/+4). */
typedef struct {
    s16 x;
    s16 y;
    s16 z;
} Point16;

/* Computes the midpoint of two Point16s, dividing by 2 with round-toward-
   zero (matches (v + (v >> 31)) >> 1 in the original asm). */
void f_main_80037F00_Midpoint16(Point16 *a, Point16 *b, Point16 *out) {
    out->x = (s16) ((a->x + b->x) / 2);
    out->y = (s16) ((a->y + b->y) / 2);
    out->z = (s16) ((a->z + b->z) / 2);
}
