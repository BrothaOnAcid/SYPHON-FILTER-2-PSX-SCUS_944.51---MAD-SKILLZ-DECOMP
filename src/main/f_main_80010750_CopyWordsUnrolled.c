#include "../../include/types.h"

/* Word copy, unrolled 4-at-a-time with a scalar remainder loop. Always
   returns 0. */
s32 f_main_80010750_CopyWordsUnrolled(u32 *dst, u32 *src, s32 count) {
    s32 c;

    c = count;
    while (c >= 4) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        dst += 4;
        src += 4;
        c -= 4;
    }
    if (c != 0) {
        do {
            *dst = *src;
            dst++;
            src++;
            c--;
        } while (c != 0);
    }
    return 0;
}
