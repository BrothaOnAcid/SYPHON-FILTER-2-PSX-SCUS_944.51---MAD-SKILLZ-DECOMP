#include "../../include/types.h"

/* D_8013490C..D_80134938: guessed table block, laid out as
   [0x00..0x0C) s16[6] TableA (reset -1)
   [0x0C..0x18) s16[6] TableB (reset 0)
   [0x18..0x20) u8[6]  TableC (reset 2, 2 pad bytes)
   [0x20..0x38) s32[6] TableD (reset 0) */
extern u8 g_main_8013490C_TableBlock[0x38];

extern u8 g_main_8011F8D0_Flag;
extern s16 g_main_8011F8AC_Id;
extern s32 g_main_801340E4_Table; /* last of 5 s32 slots, reset downward to -1 */

extern void func_800875C4(s32 a0, s16 *a1, s32 *a2, s32 a3);
extern void func_800875D4(s32 a0);

void f_init_80164650_ResetModeTables2(void) {
    s32 i;
    s16 *tableA = (s16 *) g_main_8013490C_TableBlock;
    s16 *tableB = (s16 *) (g_main_8013490C_TableBlock + 0xC);
    u8 *tableC = g_main_8013490C_TableBlock + 0x18;
    s32 *tableD = (s32 *) (g_main_8013490C_TableBlock + 0x20);

    for (i = 0; i < 6; i++) {
        tableA[i] = -1;
        tableB[i] = 0;
        tableC[i] = 2;
        tableD[i] = 0;
    }

    func_800875C4(0, (s16 *) tableC, tableD + 6, 2);
    func_800875D4(0);

    g_main_8011F8D0_Flag = 0;
    g_main_8011F8AC_Id = -1;

    {
        s32 *p32 = &g_main_801340E4_Table;
        for (i = 4; i >= 0; i--) {
            *p32 = -1;
            p32--;
        }
    }
}
