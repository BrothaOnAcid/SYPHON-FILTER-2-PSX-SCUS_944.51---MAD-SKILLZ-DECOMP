#include "../../include/types.h"

/* Audio-sequencer state (PSYQ library block, uses its own gp base
   0x8012EC64). */
extern AudioSeqState g_main_8012F41C_AudioSeq;

extern void f_main_80104F28_UnlinkSeq(SeqNode *node);  /* decompiled unlink */

/* Schedule tick (called at the end of each audio poll). Walks the schedule
   list (head AudioSeqState.seqHead, 0x8012F478), disabled while seqLock
   (0x8012F47C) or seqPause (0x8012F480) is set. Each node's timer counts
   down; on reaching 0 the node value is stepped by its offset and the node
   callback runs (fn(node, value)); repeating nodes (state == 1) reload their
   timer from period. A non-zero callback return unlinks the node. */
void f_main_80104E50_TickSchedule(void) {
    AudioSeqState *s = &g_main_8012F41C_AudioSeq;
    SeqNode *node;
    SeqNode *next;

    if (s->seqLock != 0 || s->seqPause != 0)
        return;
    node = s->seqHead;
    while (node != 0) {
        s32 ret = 0;

        if (node->state == 1)
            node->timer--;
        if (node->timer == 0) {
            node->value += node->offset;
            ret = node->fn(node, node->value);
            if (ret == 0 && node->state == 1)
                node->timer = node->period;
        }
        next = node->next;
        if (ret != 0)
            f_main_80104F28_UnlinkSeq(node);
        node = next;
    }
}
