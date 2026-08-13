#include "../../include/types.h"

extern u8 g_main_801279AB_Flag; /* guess: toggled around func_8002BE68 (set/cleared at several call sites) */

s32 f_main_8002BDCC_GetFlag801279AB(void) {
    return g_main_801279AB_Flag;
}
