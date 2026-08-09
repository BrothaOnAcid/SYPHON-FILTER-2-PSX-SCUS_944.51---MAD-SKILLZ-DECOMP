#include "../../include/types.h"

extern u32 g_init_8013867C_Table[0x2B];
extern u32 g_init_801385D0_Table[0x20];
extern u8 g_init_80138304_Table[0x24][0x10];
extern s8 g_main_8012B0BC_Table[0x2B];
extern s8 g_main_8012B0E7_Table[0x2B];

/* Resets several tables: zeroes 0x2B words at g_init_8013867C_Table, 0x20
   words at g_init_801385D0_Table, all 0x24 records (0x10 bytes each) of
   g_init_80138304_Table, then fills 0x2B bytes of both
   g_main_8012B0BC_Table and g_main_8012B0E7_Table with -1. */
void f_init_8015944C_ResetTables(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 0x2B; i++)
        g_init_8013867C_Table[i] = 0;

    for (i = 0; i < 0x20; i++)
        g_init_801385D0_Table[i] = 0;

    for (i = 0; i < 0x24; i++)
        for (j = 0; j < 0x10; j++)
            g_init_80138304_Table[i][j] = 0;

    for (i = 0; i < 0x2B; i++) {
        g_main_8012B0BC_Table[i] = -1;
        g_main_8012B0E7_Table[i] = -1;
    }
}
