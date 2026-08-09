#include "../../include/types.h"

extern u8 g_init_801132C8_PresetA[];
extern u8 g_init_80113560_PresetB[];
extern u8 g_main_8012A1D4_Dst[];

/* --- libc-style externs (not yet decompiled) --- */
extern void func_8002D410(void *src, void *dst, s32 count);

/* Copies/decompresses one of two fixed source tables into
   g_main_8012A1D4_Dst, selected by `flag`: nonzero -> PresetA (count 0,
   i.e. self-describing/auto length), zero -> PresetB (fixed count 0xA5). */
void f_init_80163228_LoadPreset(u8 flag) {
    if (flag != 0)
        func_8002D410(g_init_801132C8_PresetA, g_main_8012A1D4_Dst, 0);
    else
        func_8002D410(g_init_80113560_PresetB, g_main_8012A1D4_Dst, 0xA5);
}
