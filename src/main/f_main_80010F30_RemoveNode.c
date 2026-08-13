#include "../../include/types.h"
#include <string.h>

extern void func_80010C44(void *a0, void *a1, void *a2); /* not yet decompiled */

/* guess: BST-style node removal. a0 is a container whose +0x20 field points
   at the node to remove; recurses into the left subtree unconditionally,
   then either hands the node off to func_80010C44 (when it has a `next`
   sibling) or promotes it by copying its opaque payload up into *a0, then
   recurses into the right subtree carrying the merged flag. Clears the
   node's flag before returning. Returns 0x2B if a0/its node is NULL,
   otherwise 0. Handwritten in the original build (no compiler-generated
   prologue/epilogue), reused by f_main_80011084_RemoveTailNode and directly
   by 19+ other call sites across unrelated container types. */
s32 f_main_80010F30_RemoveNode(TreeNode20 *a0, s32 a1) {
    TreeNode20 *node, *left, *right, *next;
    s32 flag;

    if (a0 == 0)
        return 0x2B;
    node = a0->next;
    if (node == 0)
        return 0x2B;

    left = node->left;
    right = node->right;
    flag = node->flag;

    if (left != 0)
        f_main_80010F30_RemoveNode(left, a1);

    flag |= a1;
    next = node->next;

    if (next != 0) {
        if (flag != 0)
            func_80010C44(next, node, a0);
    } else if (flag != 0 || right == 0) {
        memcpy(a0, node, 0x20); /* promote node's opaque payload into a0's slot */
    }

    if (right != 0)
        f_main_80010F30_RemoveNode(right, flag);

    node->flag = 0;
    return 0;
}
