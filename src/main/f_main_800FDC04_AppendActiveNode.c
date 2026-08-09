#include "../../include/types.h"

/* PSYQ audio-sequencer state; this function is in the blocked region
   (0x800EFD3C-0x80168298) that uses its own gp base 0x8012EC64, not main's
   gp 0x8011EC64 - see g_main_8012F41C_AudioSeq. */
extern AudioSeqState g_main_8012F41C_AudioSeq;

/* Appends `node` to the tail of g_main_8012F41C_AudioSeq.activeHead,
   chaining through each node's +0xA4 "next" field. `node->next` is always
   cleared first. */
void f_main_800FDC04_AppendActiveNode(ActiveNode *node) {
    ActiveNode *cur;

    node->next = 0;

    if (g_main_8012F41C_AudioSeq.activeHead == 0) {
        g_main_8012F41C_AudioSeq.activeHead = node;
        return;
    }

    cur = (ActiveNode *) g_main_8012F41C_AudioSeq.activeHead;
    while (cur->next != 0)
        cur = cur->next;
    cur->next = node;
}
