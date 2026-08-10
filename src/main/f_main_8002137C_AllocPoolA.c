#include "../../include/types.h"

extern u8 g_main_8011EDE4_PoolAInit; /* guess: one-shot lazy-init flag for the pool below */
extern PoolARecord g_main_8012340C_PoolA[5];

/* guess: lazily seeds the pool's free markers on first call, then returns
   the first free slot (marker == 0xCACACACA), clearing its marker to 0;
   0 if all 5 slots are in use. */
void *f_main_8002137C_AllocPoolA(void) {
    s32 i;
    PoolARecord *p;

    if (!g_main_8011EDE4_PoolAInit) {
        g_main_8011EDE4_PoolAInit = 1;
        for (i = 4; i >= 0; i--)
            g_main_8012340C_PoolA[i].marker = (s32) 0xCACACACA;
    }

    for (i = 0, p = g_main_8012340C_PoolA; i < 5; i++, p++) {
        if (p->marker == (s32) 0xCACACACA) {
            p->marker = 0;
            return p;
        }
    }

    return 0;
}
