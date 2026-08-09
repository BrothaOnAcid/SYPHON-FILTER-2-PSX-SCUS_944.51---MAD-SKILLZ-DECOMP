#include "../../include/types.h"

/* PSYQ library routine (splat flags this as "Handwritten function" - hand
   tuned assembly, not compiler output). Classic 12-bit fixed point signed
   division: computes (a << 12) / b with an extra precision pass to avoid
   losing bits when abs(a) is large. Traps with `break 7` on divide by 0. */
s32 f_main_80010698_FixDiv(s32 a, s32 b) {
    s32 s;
    u32 r;

    s = a ^ b;

    if (b == 0) {
        r = 0x7FFFFFFF;
    } else {
        u32 hi;

        if (a < 0) {
            a = -a;
        }
        if (b < 0) {
            b = -b;
        }

        r = ((u32) a << 12) / (u32) b;

        hi = (u32) a >> 20;
        if (hi != 0) {
            r += hi * 2 * (0x80000000U / (u32) b);
        }
    }

    if (s < 0) {
        r = -(s32) r;
    }

    return (s32) r;
}
