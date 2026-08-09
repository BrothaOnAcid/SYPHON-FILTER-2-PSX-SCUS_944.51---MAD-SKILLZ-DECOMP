#include "../../include/types.h"

extern void f_main_80025D3C_RemoveNode(ListHead *head, ListNode *node);

/* Removes and frees every node in `head`'s list (via
   f_main_80025D3C_RemoveNode, which also pushes each node onto the global
   free-list), then clears the head pointer. */
void f_main_80025DE8_FreeList(ListHead *head) {
    ListNode *node = head->first;
    ListNode *next;

    while (node != 0) {
        next = node->next;
        f_main_80025D3C_RemoveNode(head, node);
        node = next;
    }

    head->first = 0;
}
