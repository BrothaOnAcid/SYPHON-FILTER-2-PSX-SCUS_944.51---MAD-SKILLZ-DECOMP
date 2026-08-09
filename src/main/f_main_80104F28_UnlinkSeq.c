#include "../../include/types.h"

/* Audio-sequencer state (PSYQ library block, uses its own gp base
   0x8012EC64). */
extern AudioSeqState g_main_8012F41C_AudioSeq;

/* Unlink `node` from the doubly-linked schedule list (head in
   AudioSeqState.seqHead, 0x8012F478). No-op unless the node is linked
   (flags != 0); clears flags/prev/next afterwards. */
void f_main_80104F28_UnlinkSeq(SeqNode *node) {
    AudioSeqState *s = &g_main_8012F41C_AudioSeq;

    if (node != 0 && node->flags != 0) {
        if (node->prev != 0)
            node->prev->next = node->next;
        else
            s->seqHead = node->next;
        if (node->next != 0)
            node->next->prev = node->prev;
        node->flags = 0;
        node->prev = 0;
        node->next = 0;
    }
}
