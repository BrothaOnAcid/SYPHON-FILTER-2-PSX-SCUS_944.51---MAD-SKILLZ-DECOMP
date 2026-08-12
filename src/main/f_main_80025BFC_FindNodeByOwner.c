#include "../../include/types.h"

/* Walks the list rooted at `head`, looking for a node whose `owner` field
   equals `owner`. Stops early and returns `stop` itself if that sentinel
   node is reached first. Returns NULL if the list ends without hitting
   either `stop` or a matching owner. */
ListNode *f_main_80025BFC_FindNodeByOwner(ListHead *head, ListNode *stop, void *owner) {
    ListNode *n = head->first;

    if (n == 0) {
        return 0;
    }
    for (;;) {
        if (n == stop) {
            return n;
        }
        if (n->owner == owner) {
            return n;
        }
        n = n->next;
        if (n == 0) {
            return 0;
        }
    }
}
