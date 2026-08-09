#include "../../include/types.h"

extern s8 *f_main_800F6FF8_strchr(const s8 *s, s32 c);

extern s32 g_main_8011EEC4_ResidentTableFlag; /* nonzero once a resident file table has been set up */

extern s32 func_80026F4C(s32 table, s8 *name);        /* looks up `name` in the resident file table, returns an index or -1 */
extern void func_80026D98(s32 table, s32 index, s32 *outSize);   /* entry size */
extern void func_80026D28(s32 table, s32 index, void **outSrc);  /* entry data pointer */
extern void *func_80025AD0(s32 size);                  /* allocator */
extern void func_80010750(void *dst, void *src, u32 wordCount); /* word copy */

/* Looks up `name` (after stripping a leading path and trailing ";n" CD
   version suffix) in the resident file table, then copies its data out.
   `mode == 0`: copy into the caller-supplied buffer `outPtr` directly.
   `mode != 0`: allocate a buffer via func_80025AD0 and hand it back through
   `*outPtr`. Returns the entry size, or 0 if not found. */
s32 f_main_8002B3D8_GetResidentFile(s8 *name, void **outPtr, u8 mode) {
    s8 *p;
    s32 i;
    s32 sz;
    void *src;
    void *dst;

    p = f_main_800F6FF8_strchr(name, '\\');
    if (p != 0) {
        name = p + 1;
    }

    p = f_main_800F6FF8_strchr(name, ';');
    if (p != 0) {
        *p = 0;
    }

    i = func_80026F4C(g_main_8011EEC4_ResidentTableFlag, name);
    if (i < 0) {
        return 0;
    }

    func_80026D98(g_main_8011EEC4_ResidentTableFlag, i, &sz);
    func_80026D28(g_main_8011EEC4_ResidentTableFlag, i, &src);

    if (mode != 0) {
        *outPtr = func_80025AD0(sz);
        dst = *outPtr;
    } else {
        dst = outPtr;
    }

    func_80010750(dst, src, (u32) sz >> 2);

    return sz;
}
