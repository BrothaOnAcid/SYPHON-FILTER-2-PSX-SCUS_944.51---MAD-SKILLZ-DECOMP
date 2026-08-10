#include "../../include/types.h"

/* not yet decompiled; the original asm temporarily swaps $sp to a fixed
   PSX scratchpad address around each of these calls (a codegen/perf
   detail not represented here). */
extern void func_80068994(void);
extern void func_800689E4(void);

/* guess: runs two unrelated void subsystem passes in sequence. */
void f_main_80069570_RunActorRecoilTriggers(void) {
    func_80068994();
    func_800689E4();
}
