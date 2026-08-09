#include "../../include/types.h"

extern u8 g_main_801382C4_TableEnd; /* guess: end-of-table sentinel; only its
    address is used, as an upper bound for valid arg0 pointers into that
    static object table */

extern void func_800A71A4(s32 id);

/* arg0: pointer into a static object table (bounds-checked against
   &g_main_801382C4_TableEnd). +0x14 is an optional linked list of nodes
   (+0x0 = id passed to func_800A71A4, +0x8 = next); if absent, clears bit 0
   of the object's own +0x0 flags word instead. Returns 0 if arg0 is out of
   range, 1 otherwise. */
s32 f_main_800A5CE8_ReleaseOrClearFlag(void *arg0) {
    void *node;

    if ((u8 *) arg0 >= &g_main_801382C4_TableEnd)
        return 0;

    node = *(void **) ((u8 *) arg0 + 0x14);
    if (node != 0) {
        do {
            func_800A71A4(*(s32 *) node);
            node = *(void **) ((u8 *) node + 0x8);
        } while (node != 0);
        return 1;
    }

    *(s32 *) arg0 &= ~1;
    return 1;
}
