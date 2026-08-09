#include "../../include/types.h"

extern void func_800F9210(s32 a0);

/* guess: forwards the low byte of `arg0` to func_800F9210 (same helper used
   by f_main_8008D59C_ReinitSound with a literal 0). */
void f_main_8008E448_SetAudioFlag(s32 arg0) {
    func_800F9210(arg0 & 0xFF);
}
