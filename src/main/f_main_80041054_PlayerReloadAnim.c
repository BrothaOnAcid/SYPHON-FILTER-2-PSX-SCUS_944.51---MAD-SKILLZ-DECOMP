#include "../../include/types.h"

extern AmmoUser *g_main_8012A574_Player;

extern void func_80042CF4(s16 weaponId, s32 mode);
extern void func_80064A74(void *player, s32 a1);
extern void func_8008E194(s32 a2);
extern void func_800A9EA0(void);

/* arg0 selects the reload animation mode: 1 -> 0x62, 0 -> 0x5C, anything
   else skips the animation and instead calls func_800A9EA0. */
void f_main_80041054_PlayerReloadAnim(s32 arg0, s32 arg2) {
    AmmoUser *player = g_main_8012A574_Player;

    func_8008E194(arg2);

    if (arg0 == 1) {
        func_80042CF4(player->weaponId, 0x62);
        func_80064A74(player, 0);
    } else if (arg0 == 0) {
        func_80042CF4(player->weaponId, 0x5C);
        func_80064A74(player, 0);
    } else {
        func_800A9EA0();
    }
}
