#include "../../include/types.h"

extern void func_800B38B0(u16 hi, u16 lo); /* not yet decompiled */

/* Splits arg0's +0xC packed 32-bit field into hi/lo 16-bit halves and
   forwards them to func_800B38B0. No callers seen yet in the disassembled
   main-module text. */
void f_main_8002A4EC_DispatchField0xC(void *arg0) {
    u32 packed = *(u32 *) ((u8 *) arg0 + 0xC);

    func_800B38B0((u16) (packed >> 16), (u16) packed);
}
