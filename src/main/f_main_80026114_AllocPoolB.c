#include "../../include/types.h"

extern u8 g_main_8011EE6C_PoolBInit; /* guess: one-shot lazy-init flag for the pool below */
extern PoolBRecord g_main_80125FF4_PoolB[5];

/* guess: same allocator pattern as f_main_8002137C_AllocPoolA, over a
   separate pool of 5 larger (0x14-byte) records. */
void *f_main_80026114_AllocPoolB(void) {
    s32 i;
    PoolBRecord *p;

    if (!g_main_8011EE6C_PoolBInit) {
        g_main_8011EE6C_PoolBInit = 1;
        for (i = 4; i >= 0; i--)
            g_main_80125FF4_PoolB[i].marker = (s32) 0xCACACACA;
    }

    for (i = 0, p = g_main_80125FF4_PoolB; i < 5; i++, p++) {
        if (p->marker == (s32) 0xCACACACA) {
            p->marker = 0;
            return p;
        }
    }

    return 0;
}
