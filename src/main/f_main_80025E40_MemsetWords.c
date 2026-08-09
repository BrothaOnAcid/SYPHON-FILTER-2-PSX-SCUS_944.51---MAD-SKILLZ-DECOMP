#include "../../include/types.h"

/* Word-at-a-time memset: replicates the low byte of `val` across a 32-bit
   word and stores it while `size >= 4`. Unlike libc memset, it does NOT
   handle the trailing 1-3 bytes if `size` isn't a multiple of 4 - a partial/
   fast variant, not a general-purpose memset. */
void f_main_80025E40_MemsetWords(void *dst, u8 val, u32 size) {
    u32 w = val | (val << 8) | (val << 16) | (val << 24);
    u32 *p = (u32 *) dst;

    while (size >= 4) {
        *p++ = w;
        size -= 4;
    }
}
