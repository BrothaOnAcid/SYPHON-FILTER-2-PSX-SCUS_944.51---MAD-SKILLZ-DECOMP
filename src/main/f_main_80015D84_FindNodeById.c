#include "../../include/types.h"

/* Linked list at +0x20: each node is { void *data; s32 pad; Node *next; }.
   data's +0x4 holds a s16 id. id < 0 matches the first node unconditionally. */
void *f_main_80015D84_FindNodeById(void *arg0, s16 id) {
    u8 *node = *(u8 **) ((u8 *) arg0 + 0x20);
    u8 *data;

    if (node == 0) {
        return 0;
    }
    do {
        data = *(u8 **) node;
        if (id < 0) {
            return data;
        }
        if (*(s16 *) (data + 4) == id) {
            return data;
        }
        node = *(u8 **) (node + 8);
    } while (node != 0);
    return 0;
}
