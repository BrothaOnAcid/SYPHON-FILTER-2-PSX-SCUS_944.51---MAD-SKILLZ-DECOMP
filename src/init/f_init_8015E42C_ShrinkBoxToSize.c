#include "../../include/types.h"

/* Guessed owner-of-a-WldRes wrapper; only field observed here is a WldRes*
   at +0x08 (matches WldModelHandle.subRes[1], meaning otherwise unconfirmed). */
typedef struct {
    u8 _pad00[0x8];
    WldRes *res;
} BoxOwner;

/* Mirror of f_init_8015E370_GrowBoxToSize: for each axis of typeDef's
   min/max (via arg0's WldRes at +8), if the current extent (max - min) is
   larger than the requested size, shrinks the box symmetrically about its
   center down to that size. No-op if arg0->res is NULL. */
void f_init_8015E42C_ShrinkBoxToSize(BoxOwner *arg0, s32 sizeX, s32 sizeY, s32 sizeZ) {
    WldRes *res;
    WldTypeDef *t;
    s32 extent, half;

    res = arg0->res;
    if (res == 0) {
        return;
    }
    t = res->typeDef;

    extent = t->max0 - t->min0;
    if (sizeX < extent) {
        half = (extent - sizeX) >> 1;
        t->min0 += half;
        t->max0 -= half;
    }

    extent = t->max1 - t->min1;
    if (sizeY < extent) {
        half = (extent - sizeY) >> 1;
        t->min1 += half;
        t->max1 -= half;
    }

    extent = t->max2 - t->min2;
    if (sizeZ < extent) {
        half = (extent - sizeZ) >> 1;
        t->min2 += half;
        t->max2 -= half;
    }
}
