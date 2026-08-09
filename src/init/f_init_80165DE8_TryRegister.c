#include "../../include/types.h"

extern s32 func_80025708(void *entry);
extern s32 func_80026E00(void);

/* a0/a1 unused; entry (a2) is the only parameter actually consumed. */
s32 f_init_80165DE8_TryRegister(void *a0, void *a1, void *entry) {
    (void) a0;
    (void) a1;

    if (func_80026E00() != 0)
        return 0;

    if (func_80025708(entry) != 0)
        return 0;

    return 1;
}
