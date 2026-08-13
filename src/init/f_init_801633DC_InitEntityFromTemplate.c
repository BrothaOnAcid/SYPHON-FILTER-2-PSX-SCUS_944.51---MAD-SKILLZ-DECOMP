#include "../../include/types.h"

extern s32 D_80158BEC[4]; /* not yet decompiled: all-zero default vector/quad constant */

extern void func_80025620(void *dst, s32 id, s32 flag, s32 *outVec); /* not yet decompiled */
extern void func_80065F90(void *src, s32 *outVec);                    /* not yet decompiled */
extern void func_80066420(void *dst, s32 *vec);                       /* not yet decompiled */

/* guess: builds a game object (`dst`, some kind of actor/entity record) from
   a template struct `src`. Copies a leading block of fields verbatim
   (words at +0x0..+0x10, then several halfwords), except `dst` has one
   extra halfword at +0x16 that `src` doesn't have, shifting the remaining
   halfword copies by 2 bytes; word copies resync at +0x20..+0x2C. Then
   resolves a base position via func_80025620(dst+4, dst's own id field,
   0, &pos) and, if `velSrc` is non-NULL, subtracts a second vector from
   func_80065F90(velSrc, &vel) out of it component-wise. Writes the
   resulting vector to dst+0x30 and the all-zero D_80158BEC constant to
   dst+0xB0 (both via func_80066420 - looks like a "set transform" helper),
   then zeroes a scattered set of trailing fields. Returns 0 if `dst` is
   NULL (no-op), else 1. */
s32 f_init_801633DC_InitEntityFromTemplate(void *dst, void *src, void *velSrc) {
    u8 *d = (u8 *) dst;
    u8 *s = (u8 *) src;
    s32 pos[4];
    s32 vel[4];
    s32 i;

    if (dst == 0) {
        return 0;
    }

    for (i = 0; i <= 0x10; i += 4) {
        *(s32 *) (d + i) = *(s32 *) (s + i);
    }

    *(s16 *) (d + 0x14) = *(s16 *) (s + 0x14);
    *(s16 *) (d + 0x18) = *(s16 *) (s + 0x16);
    *(s16 *) (d + 0x1A) = *(s16 *) (s + 0x18);
    *(s16 *) (d + 0x1C) = *(s16 *) (s + 0x1A);
    *(s16 *) (d + 0x1E) = *(s16 *) (s + 0x1C);

    for (i = 0x20; i <= 0x2C; i += 4) {
        *(s32 *) (d + i) = *(s32 *) (s + i);
    }

    func_80025620(d + 4, *(s32 *) (d + 0), 0, pos);

    if (velSrc != 0) {
        func_80065F90(velSrc, vel);
        pos[0] -= vel[0];
        pos[1] -= vel[1];
        pos[2] -= vel[2];
    }

    func_80066420(d + 0x30, pos);
    func_80066420(d + 0xB0, D_80158BEC);

    *(s32 *) (d + 0x130) = 0;
    *(s32 *) (d + 0x134) = 0;
    *(s32 *) (d + 0x138) = 0;
    *(s32 *) (d + 0x140) = 0;
    *(s32 *) (d + 0x144) = 0;
    *(s32 *) (d + 0x148) = 0;
    *(s32 *) (d + 0x150) = 0;
    *(s32 *) (d + 0x154) = 0;
    *(s32 *) (d + 0x158) = 0;
    *(s32 *) (d + 0x160) = 0;
    *(s32 *) (d + 0x164) = 0;
    *(s32 *) (d + 0x16C) = 0;

    return 1;
}
