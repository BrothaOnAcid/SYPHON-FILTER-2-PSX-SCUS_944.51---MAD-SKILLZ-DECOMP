#include "../../include/types.h"

extern void *func_80025AD0(s32 size); /* not yet decompiled: bump allocator, see f_main_80025AD0_AllocDown */
extern void func_8008FBA4(void *obj, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5); /* not yet decompiled */
extern void func_8008FA8C(void *obj, s32 a1); /* not yet decompiled */
extern void func_8008F878(void *obj, s32 arg1); /* not yet decompiled */

/* No-ops (returns 0) if arg0 is NULL or arg0->unkC is NULL. Otherwise resets
   arg0 via func_8008FBA4(-0x7FFF,-1,-1,-1,-1), allocates a fresh 0x1A8-byte
   context and stores it at arg0->unkC->unk160, zeroing most of its fields
   (0x8/0xC/0x10/0x118/0x11C/0x11D/0x11E/0x120/0x124/0x128/0x130/0x134),
   OR-ing bit 4 into its own +0x4 flags word, calling func_8008FA8C(arg0,0),
   then clearing bit 1 and setting bit 2 on that same +0x4 flags word, and
   finally func_8008F878(arg0, arg1). Returns 0 if the allocation failed,
   otherwise 1. */
s32 f_init_8016327C_InitAnimCtx(void *arg0, s32 arg1) {
    u8 *outer;
    u8 *ctx;

    if (arg0 == 0) {
        return 0;
    }
    outer = *(u8 **) ((u8 *) arg0 + 0xC);
    if (outer == 0) {
        return 0;
    }

    func_8008FBA4(arg0, -0x7FFF, -1, -1, -1, -1);

    ctx = (u8 *) func_80025AD0(0x1A8);
    *(u8 **) (outer + 0x160) = ctx;
    if (ctx == 0) {
        return 0;
    }

    *(s32 *) (ctx + 0x118) = 0;
    *(s32 *) (ctx + 0x8) = 0;
    *(s32 *) (ctx + 0xC) = 0;
    *(s32 *) (ctx + 0x10) = 0;
    *(s32 *) (ctx + 0x4) |= 4;
    *(u8 *) (ctx + 0x11C) = 0;
    *(u8 *) (ctx + 0x11D) = 0;
    *(u8 *) (ctx + 0x11E) = 0;
    *(s32 *) (ctx + 0x120) = 0;
    *(s32 *) (ctx + 0x124) = 0;
    *(s32 *) (ctx + 0x128) = 0;
    *(s32 *) (ctx + 0x130) = 0;
    *(s32 *) (ctx + 0x134) = 0;

    func_8008FA8C(arg0, 0);

    *(s32 *) (ctx + 0x4) &= ~1;
    *(s32 *) (ctx + 0x4) |= 2;

    func_8008F878(arg0, arg1);
    return 1;
}
