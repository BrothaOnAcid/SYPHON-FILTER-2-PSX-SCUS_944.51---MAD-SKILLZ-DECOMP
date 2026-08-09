#include "../../include/types.h"

extern ResetEntry g_main_8012C94C_ResetEntries[8];
extern s32 g_main_8011F614_unk;
extern u16 g_main_8011408E_unk;
extern ViewportRect *g_main_8011F6E0_Viewport;

/* Resets an 8-entry table (leading field only) to -1, clears
   g_main_8011F614_unk, and caches the current viewport's +0x4C field. */
void f_init_80164A84_ResetEntries(void) {
    s32 i;

    for (i = 7; i >= 0; i--)
        g_main_8012C94C_ResetEntries[i].unk0 = -1;

    g_main_8011F614_unk = 0;
    g_main_8011408E_unk = g_main_8011F6E0_Viewport->unk4C;
}
