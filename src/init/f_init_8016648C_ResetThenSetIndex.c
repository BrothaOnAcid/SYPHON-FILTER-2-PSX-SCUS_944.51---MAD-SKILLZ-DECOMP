#include "../../include/types.h"

extern void f_init_80166158_BuildPatchGrid(void *arg0);
extern void func_801663E0(s32 index);

/* NOTE: arg0 is passed straight through to f_init_80166158_BuildPatchGrid
   unchanged (the original asm never sets $a0 before that call, so it's
   whatever this function's own arg0 already was) - confirms
   f_init_80166158_BuildPatchGrid actually reads its arg0. */
void f_init_8016648C_ResetThenSetIndex(s32 arg0) {
    f_init_80166158_BuildPatchGrid((void *) (u64) arg0);
    func_801663E0(arg0);
}
