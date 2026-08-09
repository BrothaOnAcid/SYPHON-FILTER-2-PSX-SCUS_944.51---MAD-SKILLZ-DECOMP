#include "../../include/types.h"

extern s32 g_main_80134904_Table;          /* last of 11 s32 slots, reset downward to -1 */
extern s16 g_main_8011F8CC_Table2;         /* last of 3 s16 slots, reset downward to -1 */
extern u8 g_main_8011F8C4_Flag;
extern u8 g_main_8011F87C_Flag;
extern s16 g_main_8011F890_Id;
extern s16 g_main_8011F8A8_Id;
extern s32 g_main_8011F898_Val;            /* reset to -0x28 */
extern s16 g_main_8011F8C0_Id;
extern s16 g_main_8011F870_Id;
extern s16 g_main_8011F874_Id;
extern u8 g_main_8011F868_Flag;
extern u8 g_main_8011F880_Flag;
extern u8 g_main_8011F89C_Val;             /* reset to 0x8C */
extern s16 g_main_8011F858_Id;
extern s16 g_main_8011F894_Id;
extern s16 g_main_8011F860_Id;
extern s16 g_main_8011F8BC_Val;            /* reset to 0x800 */
extern s16 g_main_8011F8D8_Id;
extern s32 g_main_8011F888_Val;
extern s32 g_main_8011F88C_Val;
extern u8 g_main_801340E8_Entries[5][0x190]; /* stride 0x190 */

extern void func_80083540(s32 a0, s32 a1);
extern void func_80066900(void *entry);

void f_init_8016450C_ResetModeTables(void) {
    s32 i;
    s32 *p32;
    s16 *p16;

    p32 = &g_main_80134904_Table;
    for (i = 10; i >= 0; i--) {
        *p32 = -1;
        p32--;
    }

    p16 = &g_main_8011F8CC_Table2;
    for (i = 2; i >= 0; i--) {
        *p16 = -1;
        p16--;
    }

    g_main_8011F8C4_Flag = 0;
    g_main_8011F87C_Flag = 0;
    g_main_8011F890_Id = -1;
    g_main_8011F8A8_Id = -1;
    g_main_8011F898_Val = -0x28;
    g_main_8011F8C0_Id = -1;
    g_main_8011F870_Id = -1;
    g_main_8011F874_Id = -1;
    g_main_8011F868_Flag = 0;
    g_main_8011F880_Flag = 0;

    func_80083540(0, -1);

    g_main_8011F89C_Val = 0x8C;
    g_main_8011F858_Id = -1;
    g_main_8011F894_Id = -1;
    g_main_8011F860_Id = -1;
    g_main_8011F8BC_Val = 0x800;
    g_main_8011F8D8_Id = -1;

    for (i = 0; i < 5; i++)
        func_80066900(g_main_801340E8_Entries[i]);

    g_main_8011F888_Val = -1;
    g_main_8011F88C_Val = -1;
}
