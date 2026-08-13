#include "../../include/types.h"

extern ListNode *g_main_8011EE38_FreeListHead;
extern u16 g_main_8011EE3C_ActiveNodeCount;

/* Counterpart to f_main_80025D3C_RemoveNode: pops a node off the global
   free-list (g_main_8011EE38_FreeListHead), sets its owner to `owner`, and
   pushes it onto the front of the doubly-linked list rooted at `head`.
   Bumps the active-node counter and returns the new node. No-ops and
   returns NULL if the free-list is empty. */
ListNode *f_main_80025C3C_AddNode(ListHead *head, void *owner) {
    ListNode *node = g_main_8011EE38_FreeListHead;
    ListNode *next;

    if (node == 0) {
        return 0;
    }

    next = node->next;
    g_main_8011EE38_FreeListHead = next;
    next->prev = 0;

    node->owner = owner;
    node->prev = 0;
    node->next = head->first;
    if (head->first != 0) {
        head->first->prev = node;
    }
    head->first = node;

    g_main_8011EE3C_ActiveNodeCount++;
    return head->first;
}
