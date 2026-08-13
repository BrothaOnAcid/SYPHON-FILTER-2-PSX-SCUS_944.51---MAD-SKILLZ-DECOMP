#include "../../include/types.h"

/* Same wrapper shape as f_init_8015E370_GrowBoxToSize's BoxOwner. */
typedef struct {
    u8 _pad00[0x8];
    WldRes *res;
} BoxOwner;

/* Reads a raw s16[9] vector at res->tag (NOTE: reinterprets the
   already-documented WldRes.tag field as a pointer here rather than an id -
   left as a raw offset access instead of renaming that field). Only
   indices 3/4/5 matter here -
   the rest are computed/stored to a stack scratch array by the compiled
   code but never read back, so that dead-store bookkeeping is omitted).
   Picks whichever of |v[3]|, |v[4]|, |v[5]| is the MEDIAN absolute value
   (median = min(a3, max(a4, a5))) and grows the matching WldTypeDef bbox
   axis (X for v[3], Y for v[4], Z for v[5] - see WldTypeDef/
   f_init_8015E370_GrowBoxToSize) toward `arg1`, clamped to non-negative
   growth, on whichever side the corresponding raw value's sign selects. */
void f_init_8015E4E8_GrowDominantAxis(BoxOwner *owner, s32 arg1) {
    s16 *v;
    WldTypeDef *t;
    s16 raw3, raw4, raw5;
    s32 abs3, abs4, abs5, median;
    s32 delta;

    if (owner->res == 0) {
        return;
    }
    v = *(s16 **) ((u8 *) owner->res + 0xC);
    t = owner->res->typeDef;

    raw3 = v[3];
    raw4 = v[4];
    raw5 = v[5];

    abs3 = raw3 < 0 ? -raw3 : raw3;
    abs4 = raw4 < 0 ? -raw4 : raw4;
    abs5 = raw5 < 0 ? -raw5 : raw5;

    median = abs4 > abs5 ? abs4 : abs5;
    if (abs3 < median) {
        median = abs3;
    }

    if (median == abs3) {
        delta = arg1 - (t->max0 - t->min0);
        if (delta < 0) {
            delta = 0;
        }
        if (raw3 <= 0) {
            t->max0 += delta;
        } else {
            t->min0 -= delta;
        }
        return;
    }
    if (median == abs4) {
        delta = arg1 - (t->max1 - t->min1);
        if (delta < 0) {
            delta = 0;
        }
        if (raw4 >= 0) {
            t->min1 -= delta;
        } else {
            t->max1 += delta;
        }
        return;
    }
    if (median == abs5) {
        delta = arg1 - (t->max2 - t->min2);
        if (delta < 0) {
            delta = 0;
        }
        if (raw5 <= 0) {
            t->max2 += delta;
        } else {
            t->min2 -= delta;
        }
    }
}
