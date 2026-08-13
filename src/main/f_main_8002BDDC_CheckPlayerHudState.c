#include "../../include/types.h"

extern u8 g_main_8011F6D0_Flag;      /* guess: gate flag, purpose unknown */
extern AmmoUser *g_main_8012A574_Player;

extern void func_800609A8(s32 a0, s32 a1); /* not yet decompiled */

/* If the player exists, the gate flag is clear, and the state word at
   (*(player->hud + 0xE0))->unk134 isn't 0x11, fire func_800609A8(0, 1). */
void f_main_8002BDDC_CheckPlayerHudState(void) {
    AmmoUser *p = g_main_8012A574_Player;
    void *sub;

    if (p != 0 && g_main_8011F6D0_Flag == 0) {
        sub = *(void **) ((u8 *) p->hud + 0xE0);
        if (*(s32 *) ((u8 *) sub + 0x134) != 0x11) {
            func_800609A8(0, 1);
        }
    }
}
