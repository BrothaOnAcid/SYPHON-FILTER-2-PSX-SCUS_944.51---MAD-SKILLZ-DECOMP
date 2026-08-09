#include "../../include/types.h"

extern void func_8002A094(void);

/* Tail-call trampoline: jumps straight into func_8002A094 with no frame. */
void f_main_80010000_JumpTramp(void) {
    func_8002A094();
}
