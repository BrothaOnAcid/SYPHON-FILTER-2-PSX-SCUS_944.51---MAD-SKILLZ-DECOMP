#include "../../include/types.h"

extern s32 g_main_80169B98_DispatchCount;   /* guess: entry count for the parallel tables below */
extern u8 g_main_80169BB4_DispatchFlags[];  /* guess: per-entry flag selecting which handler to call */

/* guess: parallel table starting right after the count word (stride 8: a
   u8 at +0x14 and a s16 at +0x4, indexed in lock-step). */

extern void func_8008D21C(u8 a, s16 b, s32 c, s32 d); /* not yet decompiled */
extern void func_8008EAB8(u8 a, s16 b, s32 c, s32 d); /* not yet decompiled */

/* guess: for each of g_main_80169B98_DispatchCount entries, calls one of two
   handlers depending on g_main_80169BB4_DispatchFlags[i] (0 -> func_8008D21C,
   else -> func_8008EAB8), passing a u8 from +0x14 and a s16 from +0x4 of the
   entry's own table (both tables live right after the count word), with the
   last two args always 0. */
void f_init_8015DC3C_DispatchByFlagTable(void) {
    s16 *arg1;
    u8 *arg0;
    s32 i;
    s32 n;

    n = g_main_80169B98_DispatchCount;
    arg1 = (s16 *) (&g_main_80169B98_DispatchCount + 1);
    arg0 = ((u8 *) &g_main_80169B98_DispatchCount) + 0x14;
    for (i = 0; i < n; i++) {
        if (g_main_80169BB4_DispatchFlags[i] == 0) {
            func_8008D21C(*arg0, *arg1, 0, 0);
        } else {
            func_8008EAB8(*arg0, *arg1, 0, 0);
        }
        arg1++;
        arg0++;
    }
}
