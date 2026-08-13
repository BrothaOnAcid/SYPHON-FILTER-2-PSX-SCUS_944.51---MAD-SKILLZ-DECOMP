#include "../../include/types.h"

extern u8 D_8011F1E8;

/* Dead/unreachable code: splat's boundary detection folds these bytes into
   func_801623B0's range (no glabel of its own, and no `j`/`jal` anywhere in
   asm/init/0.s targets 80162514), but it has a clean, self-contained
   prologue-less body and its own jr $ra, so it's kept here as its own
   function rather than force-fit into f_init_801623B0_BuildIndexTables. */
s32 f_init_80162514_SetD_8011F1E8(void) {
    D_8011F1E8 = 1;
    return 1;
}
