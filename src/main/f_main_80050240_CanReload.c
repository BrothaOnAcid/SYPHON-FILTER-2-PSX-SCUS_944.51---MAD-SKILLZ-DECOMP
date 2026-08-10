#include "../../include/types.h"

/* guess: gates a reload/fire-ready action. True only when the HUD ammo
   sub-block (hud +0xDC) is not in state 5 and its +0x1F4 counter is 2 or
   3, the ammo-user's mode byte (flags[0xC], see AmmoUser.flags) is 1, 2
   or 9, and the actor core isn't flagged busy (+0x154 bit 0x4000000). */
s32 f_main_80050240_CanReload(AmmoUser *user) {
    u8 *sub = *(u8 **) ((u8 *) user->hud + 0xDC);
    u8 mode;

    if (*(s32 *) (sub + 0x8) == 5)
        return 0;

    if ((u32) (*(s32 *) (sub + 0x1F4) - 2) >= 2)
        return 0;

    mode = user->flags[0xC];
    if (mode != 1 && mode != 2 && mode != 9)
        return 0;

    if (user->core->flags154 & 0x4000000)
        return 0;

    return 1;
}
