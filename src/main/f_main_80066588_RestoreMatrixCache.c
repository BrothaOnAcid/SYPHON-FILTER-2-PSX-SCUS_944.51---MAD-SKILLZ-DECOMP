#include "../../include/types.h"

/* guess: 0x80-byte object with a live 16-word (4x4 fixed-matrix-shaped)
   block at +0x00 and a cached/shadow copy at +0x40. */
typedef struct {
    s32 mat[16];   /* +0x00 */
    s32 cache[16]; /* +0x40 */
} MatrixCache;

/* guess: restores the live matrix from the cached copy; always returns 1. */
s32 f_main_80066588_RestoreMatrixCache(MatrixCache *obj) {
    s32 i;

    for (i = 0; i < 16; i++)
        obj->mat[i] = obj->cache[i];

    return 1;
}
