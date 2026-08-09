#include "../../include/types.h"

/* Not in asm/800.s: 0x801050B4 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c.

   Reveals two more SeqNode fields inside the previously-unlabeled
   _pad08[0x8] gap: +0x08 and +0x0C, both compared here against (tree,
   idx). Called from f_main_800FA404_DispatchSongSlot(tree, idx) with its
   own two arguments passed straight through (no new values loaded at the
   call site). */

extern AudioSeqState g_main_8012F41C_AudioSeq;
extern void f_main_80104F28_UnlinkSeq(SeqNode *node);

/* guess: unlinks every scheduled SeqNode tagged with (tree, idx) (its
   +0x8/+0xC fields) from g_main_8012F41C_AudioSeq.seqHead - i.e. cancels
   any pending scheduled callback(s) for this song-tree slot. Walks the
   whole list (no early exit on first match). */
void f_main_801050B4_CancelScheduledSlot(void *tree, s32 idx) {
    SeqNode *node;

    if (tree == 0) {
        return;
    }

    node = g_main_8012F41C_AudioSeq.seqHead;
    g_main_8012F41C_AudioSeq.seqLock = 1;

    while (node != 0) {
        u8 *raw = (u8 *) node;
        SeqNode *next = node->next;

        if (*(void **) (raw + 8) == tree && *(s32 *) (raw + 0xC) == idx) {
            f_main_80104F28_UnlinkSeq(node);
        }
        node = next;
    }

    g_main_8012F41C_AudioSeq.seqLock = 0;
}
