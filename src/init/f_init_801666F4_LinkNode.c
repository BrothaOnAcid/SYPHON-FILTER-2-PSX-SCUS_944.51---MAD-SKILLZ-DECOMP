#include "../../include/types.h"

/* Links arg1 into the list anchored at arg0->unk20: sets the anchor's
   +0x20 (head) to arg1, copies arg1's own anchor tail (+0x28) into the
   first anchor's +0x24, then makes arg1's anchor point back at arg0. */
s32 f_init_801666F4_LinkNode(void *arg0, void *arg1) {
    void *anchor0 = *(void **) ((u8 *) arg0 + 0x20);
    void *anchor1 = *(void **) ((u8 *) arg1 + 0x20);

    *(void **) ((u8 *) anchor0 + 0x20) = arg1;
    *(void **) ((u8 *) anchor0 + 0x24) = *(void **) ((u8 *) anchor1 + 0x28);
    *(void **) ((u8 *) anchor1 + 0x28) = arg0;

    return 0;
}
