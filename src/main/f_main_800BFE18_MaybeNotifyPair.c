#include "../../include/types.h"

extern void *g_main_8012A574_Player;   /* guess: player 1 character object */
extern void *g_main_8012AAD0_Player;   /* guess: player 2 character object */

/* --- libc-style externs (not yet decompiled) --- */
extern void func_800BF0FC(AmmoUser *a0);
extern void func_800BFBCC(AmmoUser *a0);

/* Fires two handlers when `user` is a player, or when a type-exclusion bit
   (index 0x1E, offset by the low 2 tag bits of `flags` * 8 - same tag scheme
   as f_main_800644AC_HandleAmmo) is set in the bitmap pointed to by
   `flags & ~3`. */
void f_main_800BFE18_MaybeNotifyPair(AmmoUser *user) {
    u32 tag;
    u8 *base;
    u32 bit;
    s32 hit;

    if (user == (AmmoUser *) g_main_8012A574_Player ||
        user == (AmmoUser *) g_main_8012AAD0_Player) {
        hit = 1;
    } else {
        tag = PTR_U32(user->flags) & 3;
        base = (u8 *) ((u64) (PTR_U32(user->flags) & ~3U));
        bit = (tag != 0) ? tag * 8 + 0x1E : 0x1E;
        hit = (*(u32 *) (base + ((bit >> 5) << 2)) & (1u << (bit & 0x1F))) != 0;
    }

    if (hit) {
        func_800BF0FC(user);
        func_800BFBCC(user);
    }
}
