#include "../../include/types.h"

/* Blocked-region code (0x800EFD3C-0x80168298, see DOC.md), hand-decoded
   from raw words with toolz/decode_mips.py; uses the alternate gp base
   0x8012EC64. Part of the "resident" streamed-VH memory allocator - see
   ResidentFreeNode's doc comment in types.h. */

extern ResidentFreeNode *g_main_8012F484_ResidentFreeHead;
extern void *f_main_801057B8_InsertFreeNode(ResidentFreeNode *n);
extern s32 g_main_8012F488_ResidentFreeCount;

/* Removes `n` from the global address-sorted free list (head/prev/next via
   g_main_8012F484_ResidentFreeHead/+0xC/+0x14), re-linking around it (if
   `n` had duplicate-size siblings threaded via +0x10, the first sibling
   takes over `n`'s slot in the primary list). Then re-inserts whatever was
   chained off `n`'s own +0x10 secondary list back into the allocator via
   f_main_801057B8_InsertFreeNode, clears `n`'s links, and decrements the
   free-node count. */
void f_main_80105700_UnlinkFreeNode(ResidentFreeNode *n) {
    ResidentFreeNode *sibling;

    if (n == 0)
        return;

    if (n == g_main_8012F484_ResidentFreeHead) {
        g_main_8012F484_ResidentFreeHead = n->next;
    } else {
        ResidentFreeNode *p = n->prev;

        if (p->bucketNext == n)
            p->bucketNext = n->next;
        else
            p->next = n->next;
    }

    sibling = n->next;
    if (sibling != 0)
        sibling->prev = n->prev;

    f_main_801057B8_InsertFreeNode(n->bucketNext);

    n->inList = 0;
    n->bucketNext = 0;
    n->next = 0;
    g_main_8012F488_ResidentFreeCount--;
}
