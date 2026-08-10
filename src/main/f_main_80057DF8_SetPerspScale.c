#include "../../include/types.h"

extern s16 g_main_8011470C_PerspScaleA; /* guess: projection depth-scale term */
extern s16 g_main_8011470E_PerspScaleB; /* guess: projection depth-scale term */

/* guess: derives a pair of fixed-point perspective/depth-scale constants
   from `arg0` (scaled by 32 first); values are consistent with a
   near/far-plane style A/B term pair. */
void f_main_80057DF8_SetPerspScale(s32 arg0) {
    s32 scaled = arg0 << 5;

    g_main_8011470C_PerspScaleA = (s16) (0x18000 / scaled);
    g_main_8011470E_PerspScaleB = (s16) ((s32) 0xFFFEC000 / scaled);
}
