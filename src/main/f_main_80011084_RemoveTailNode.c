#include "../../include/types.h"

extern s32 f_main_80010F30_RemoveNode(TreeNode20 *a0, s32 a1);

/* guess: widely-shared utility (19+ call sites across unrelated struct
   types, e.g. AngBlock.unk20) that scans a's "next" (+0x20) chain to find
   the node just before the tail, then hands it to f_main_80010F30_RemoveNode
   to detach/rebalance. Returns 0 for an empty chain, 0x2B for a single-node
   chain, otherwise f_main_80010F30_RemoveNode's result. */
s32 f_main_80011084_RemoveTailNode(TreeNode20 *a) {
    TreeNode20 *p = a;
    TreeNode20 *n;

    if (a == 0)
        return 0;

    for (;;) {
        n = p->next;
        if (n == 0)
            return 0x2B;
        if (n->next == 0)
            break;
        p = n->next;
    }
    return f_main_80010F30_RemoveNode(p, 0);
}
