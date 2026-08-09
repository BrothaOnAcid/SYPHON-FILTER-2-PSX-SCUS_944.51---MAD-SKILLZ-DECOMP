#include "../../include/types.h"

/* arg0->unk20: object whose +0x25C/+0x260 s32 counters get reset to 0x33B. */
void f_init_801664BC_ResetCounters(void *arg0) {
    void *obj = *(void **) ((u8 *) arg0 + 0x20);

    *(s32 *) ((u8 *) obj + 0x25C) = 0x33B;
    *(s32 *) ((u8 *) obj + 0x260) = 0x33B;
}
