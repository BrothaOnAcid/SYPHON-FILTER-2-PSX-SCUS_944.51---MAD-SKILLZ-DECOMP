#include "../../include/types.h"

/* Guessed owner-of-a-WldRes wrapper; only field observed here is a WldRes*
   at +0x08 (matches WldModelHandle.subRes[1], meaning otherwise unconfirmed). */
typedef struct {
    u8 _pad00[0x8];
    WldRes *res;
} BoxOwner;

/* For each axis of typeDef->min/max (via arg0's WldRes at +8), if the
   current extent (max - min) is smaller than the requested size, grows the
   box symmetrically about its center until it reaches that size. No-op if
   arg0->res is NULL. See f_init_8015E42C_ShrinkBoxToSize for the mirror
   (shrink) operation and WldTypeDef for the confirmed min/max fields. */
void f_init_8015E370_GrowBoxToSize(BoxOwner *arg0, s32 sizeX, s32 sizeY, s32 sizeZ) {
    WldRes *res;
    WldTypeDef *t;
    s32 extent, half;

    res = arg0->res;
    if (res == 0) {
        return;
    }
    t = res->typeDef;

    extent = t->max0 - t->min0;
    if (extent < sizeX) {
        half = (sizeX - extent) >> 1;
        t->min0 -= half;
        t->max0 += half;
    }

    extent = t->max1 - t->min1;
    if (extent < sizeY) {
        half = (sizeY - extent) >> 1;
        t->min1 -= half;
        t->max1 += half;
    }

    extent = t->max2 - t->min2;
    if (extent < sizeZ) {
        half = (sizeZ - extent) >> 1;
        t->min2 -= half;
        t->max2 += half;
    }
}
