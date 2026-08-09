#include "../../include/types.h"

/* Inserts `node` into the singly-linked ResourceSub list at `*head`,
   walking `index` links from the head (or to the tail if `index < 0`) and
   splicing `node` in right after the stop point. */
s32 f_init_801636D0_InsertAtIndex(ResourceSub *node, s32 index, ResourceSub **head) {
    ResourceSub *prev;
    ResourceSub *cur;
    s32 i;

    prev = 0;
    cur = *head;
    i = 0;

    if (cur != 0) {
        while (i < index || index < 0) {
            prev = cur;
            cur = cur->next;
            i++;
            if (cur == 0)
                break;
        }
    }

    if (prev != 0)
        prev->next = node;
    else
        *head = node;

    node->next = cur;
    return 1;
}
