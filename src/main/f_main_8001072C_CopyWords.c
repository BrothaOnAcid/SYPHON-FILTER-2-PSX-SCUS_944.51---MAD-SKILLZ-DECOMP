#include "../../include/types.h"

/* Copies `count` words from src to dst. Always returns 0. */
s32 f_main_8001072C_CopyWords(u32 *dst, u32 *src, s32 count) {
    u32 *end;

    end = src + count;
    do {
        *dst = *src;
        src++;
        dst++;
    } while (src != end);
    return 0;
}
