#include "../../include/types.h"

/* Blocked-region code (0x800EFD3C-0x80168298, see DOC.md), hand-decoded
   from raw words with toolz/decode_mips.py; uses the alternate gp base
   0x8012EC64. Part of the "resident" streamed-VH memory allocator - see
   ResidentFreeNode's doc comment in types.h.

   CAVEAT: the scan-and-insert logic below is transcribed for control-flow
   accuracy, but the exact ordering invariant of the list (address? size?
   a mix, given the separate 17-bucket size-class table used elsewhere in
   this cluster) is not confirmed - treat the "smaller size becomes a
   sibling of the first node it's smaller than" behavior as descriptive of
   the asm, not as a verified design intent. */

extern ResidentFreeNode *g_main_8012F484_ResidentFreeHead;
extern s32 g_main_8012F488_ResidentFreeCount;

void f_main_801057B8_InsertFreeNode(ResidentFreeNode *n) {
    ResidentFreeNode *cur;

    if (n == 0)
        return;

    n->inList = 1;
    g_main_8012F488_ResidentFreeCount++;

    cur = g_main_8012F484_ResidentFreeHead;
    if (cur == 0) {
        g_main_8012F484_ResidentFreeHead = n;
        n->prev = 0;
        return;
    }

    for (;;) {
        if (n->size < cur->size) {
            if (cur->bucketNext == 0) {
                cur->bucketNext = n;
                n->prev = cur;
                return;
            }
            cur = cur->bucketNext;
        } else {
            if (cur->next == 0) {
                cur->next = n;
                n->prev = cur;
                return;
            }
            cur = cur->next;
        }
    }
}
