#include "../../include/types.h"

extern u8 g_main_8012EBE4_EntryBuf[]; /* two entries used here, +0x00 and +0x2C */
extern s32 g_main_80168A88_Val;
extern void *g_main_80168A8C_Ptr;

extern s32 f_init_80165DE8_TryRegister(void *a0, void *a1, void *entry);

void f_init_80166100_RegisterEntries(s32 arg0) {
    f_init_80165DE8_TryRegister((void *) (u64) arg0, (void *) (u64) g_main_80168A88_Val, g_main_8012EBE4_EntryBuf);
    f_init_80165DE8_TryRegister((void *) (u64) arg0, g_main_80168A8C_Ptr, g_main_8012EBE4_EntryBuf + 0x2C);
}
