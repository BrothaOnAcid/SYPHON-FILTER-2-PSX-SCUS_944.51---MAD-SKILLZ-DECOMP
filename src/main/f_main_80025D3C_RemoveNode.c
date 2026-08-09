#include "../../include/types.h"

extern ListNode *g_main_8011EE38_FreeListHead;
extern u16 g_main_8011EE3C_ActiveNodeCount;

/* Unlinks `node` from the doubly-linked list rooted at `head`, then pushes
   it onto a global free-list (g_main_8011EE38_FreeListHead) and decrements
   the active-node counter. No-ops if `node` is NULL, `head` is empty, or
   `node` fails a sanity check (looks like a valid PSX RAM heap pointer:
   top byte 0x80, within the 0x80000000-0x801FF000 heap range, 4-byte
   aligned) - guards against double-free / corrupt pointers. */
void f_main_80025D3C_RemoveNode(ListHead *head, ListNode *node) {
    ListNode *prev;
    ListNode *next;

    if (node == 0 || head->first == 0) {
        return;
    }
    if ((PTR_U32(node) >> 24) != 0x80) {
        return;
    }
    if ((PTR_U32(node) & 0xFFFFFF) <= 0xFFFF) {
        return;
    }
    if (PTR_U32(node) > 0x801FF000) {
        return;
    }
    if (PTR_U32(node) & 3) {
        return;
    }

    prev = node->prev;
    next = node->next;

    if (prev == 0) {
        head->first = next;
    } else {
        prev->next = next;
    }

    if (next != 0) {
        next->prev = prev;
    }

    prev = g_main_8011EE38_FreeListHead;
    g_main_8011EE38_FreeListHead = node;
    prev->prev = node;
    node->prev = 0;
    node->next = prev;
    g_main_8011EE3C_ActiveNodeCount--;
}
