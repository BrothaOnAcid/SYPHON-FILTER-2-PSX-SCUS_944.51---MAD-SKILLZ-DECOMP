#include "../../include/types.h"

/* Blocked-region code (0x800EFD3C-0x80168298, see DOC.md), hand-decoded
   from raw words with toolz/decode_mips.py; uses the alternate gp base
   0x8012EC64. Part of the "resident" streamed-VH memory allocator - see
   ResidentFreeNode's doc comment in types.h.

   Walks `n`'s `next` chain (tail-recursive in the original asm) looking
   for the first node whose size is >= `want`. */
ResidentFreeNode *f_main_80105844_FindFreeNode(ResidentFreeNode *n, s32 want) {
    while (n != 0) {
        if (n->size < want)
            n = n->next;
        else
            return n;
    }
    return 0;
}
