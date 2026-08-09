#include "../../include/types.h"

extern s16 g_main_8011ED0C_Count;
extern s16 g_main_8011ED0E_Flag;
extern s32 *g_main_8011F4B8_TableA; /* [Count], zeroed here */
extern s32 *g_main_8011F4D4_TableB; /* [Count], allocated but left as-is */

extern void *f_main_80025AD0_AllocDown(s32 size);

void f_init_801664D4_AllocResetTables(void) {
    s32 i;

    g_main_8011F4B8_TableA = (s32 *) f_main_80025AD0_AllocDown(g_main_8011ED0C_Count * 4);
    g_main_8011F4D4_TableB = (s32 *) f_main_80025AD0_AllocDown(g_main_8011ED0C_Count * 4);
    g_main_8011ED0E_Flag = 0;

    for (i = 0; i < g_main_8011ED0C_Count; i++)
        g_main_8011F4B8_TableA[i] = 0;
}
