#include "../../include/types.h"

extern void func_80017624(void *inner); /* not yet decompiled, see f_init_8015EE20_AttachWeaponLink */
extern void func_8001768C(void *inner, s32 a, s32 b, s32 c); /* not yet decompiled */

/* guess: small init helper for a weapon-link node (same func_80017624 used
   by f_init_8015EE20_AttachWeaponLink's inner 0x50-byte block). `arg0` is
   loaded but never actually used by the body. This is the first of
   several functions splat merged into one "nonmatching func_80160BA4,
   0x2C0" block (like the documented boundary bugs at 0x80168298 and
   0x80166D8C); the real func_80160BA4 ends at 0x80160BF0 (jr ra), and a
   fresh prologue for the next function starts right after at 0x80160BF4 -
   not decompiled yet. */
void f_init_80160BA4_InitWeaponLinkNode(s32 arg0, void *b, s32 c) {
    func_80017624(b);
    func_8001768C(b, 0x64, 0xE, 4);
    *(s32 *) ((u8 *) b + 0x1C) = c;
}
