#include "../../include/types.h"

extern void f_main_80025E40_MemsetWords(void *dst, u8 val, u32 size);

/* Zeroes a 0x374-byte state block starting at g_init_80114704_State (its
   first field is g_init_80114704_CurObj, per
   f_init_8015CA60_BuildCommandTable), then marks two fields within it
   (+0xC, +0x10) as "unset" (-1). */
extern u8 g_init_80114704_State[0x374];
extern s32 g_init_80114710_unk;
extern s32 g_init_80114714_unk;

void f_init_801651A8_ResetModeState(void) {
    f_main_80025E40_MemsetWords(g_init_80114704_State, 0, 0x374);
    g_init_80114710_unk = -1;
    g_init_80114714_unk = -1;
}
