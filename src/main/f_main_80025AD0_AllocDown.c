#include "../../include/types.h"

/* Downward-growing bump-allocator "current top" pointer - previously
   guessed as a generic context handle (f_main_80025B3C_SetContextPtr);
   this function clarifies it's actually the allocator's watermark. */
extern void *g_main_8011EE2C_ContextPtr;
extern void *f_main_800F4190_memset(void *dst, s32 fillValue, s32 size);

/* Bump-allocates `abs(size)` bytes by moving g_main_8011EE2C_ContextPtr
   down and 4-byte-aligning the result. A negative `size` allocates the
   same but skips zero-filling the block; `size == 0` returns 0 without
   touching the pointer. */
void *f_main_80025AD0_AllocDown(s32 size) {
    s32 n;
    u8 zeroFill;
    u8 *top;

    n = size;
    zeroFill = 1;
    if (n < 0) {
        zeroFill = 0;
        n = -n;
    }

    top = (u8 *) g_main_8011EE2C_ContextPtr - n;
    if (n == 0)
        return 0;

    top -= PTR_U32(top) & 3;
    g_main_8011EE2C_ContextPtr = top;

    if (zeroFill)
        f_main_800F4190_memset(top, 0, n);

    return top;
}
