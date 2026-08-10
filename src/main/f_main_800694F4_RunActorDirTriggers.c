#include "../../include/types.h"

/* not yet decompiled; the original asm temporarily swaps $sp to a fixed
   PSX scratchpad address around each of these calls (a codegen/perf
   detail not represented here). */
extern void func_800688EC(void);
extern void func_8006ACD8(void);
extern void func_80068940(void);

/* guess: runs three unrelated void subsystem passes in sequence. */
void f_main_800694F4_RunActorDirTriggers(void) {
    func_800688EC();
    func_8006ACD8();
    func_80068940();
}
