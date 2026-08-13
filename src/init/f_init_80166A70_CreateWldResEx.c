#include "../../include/types.h"

extern u32 D_8010C1E4[4]; /* not yet decompiled: default WldTypeDef bbox block, main-module rodata */
extern u32 D_8010C1F4[4]; /* not yet decompiled: default WldTypeDef bbox block, main-module rodata */

extern void *func_80025AD0(s32 size);                                        /* not yet decompiled */
extern WldTypeDef *f_init_80166568_InternTypeDef(s32 key, WldRes *res, s32 flags);
extern void func_80024094(void *ctx, s32 arg1);                              /* not yet decompiled */
extern void func_801669CC(WldResEx *req, s32 key, s32 arg1, s32 weight);      /* not yet decompiled */

/* Allocates a WldResEx (0x2C bytes, an extended WldRes) and its EnvBlock
   (0x14 bytes), interns the WldTypeDef via f_init_80166568_InternTypeDef
   with the "eager" flag (0x400000) set (OR'ing 0x02000000 into its flags28
   too when `flag` is set), seeds the typeDef's bbox fields from two fixed
   0x10-byte template blocks (D_8010C1E4/D_8010C1F4), zero-initializes the
   request (unk09=0x80, typeDef pointer stashed at the usual WldRes.typeDef
   slot, unk1C -> EnvBlock with its three timing fields set to 500), then
   hands off to func_801669CC for the rest of the setup. Writes the
   finished request through `out`. */
void f_init_80166A70_CreateWldResEx(s32 key, s32 arg1, s32 arg2, u8 flag, WldResEx **out, s32 weight) {
    WldResEx *req = (WldResEx *) func_80025AD0(0x2C);
    EnvBlock *env = (EnvBlock *) func_80025AD0(0x14);
    WldTypeDef *typeDef = f_init_80166568_InternTypeDef(key, &req->base, 0x400000);
    s32 i;

    if (flag != 0) {
        typeDef->flags28 |= 0x02000000;
    }

    for (i = 0; i < 4; i++) {
        ((u32 *) typeDef)[i] = D_8010C1E4[i];
        ((u32 *) typeDef)[i + 4] = D_8010C1F4[i];
    }

    func_80024094((u8 *) req + 0xC, 0);

    req->base.unk00 = 0;
    req->base.unk04 = 0;
    req->base.flags8 = 0;
    req->base.unk09 = 0x80;
    req->base.unk0A = 0;
    req->base.flagsB = 0;
    req->base.typeDef = typeDef;
    req->unk28 = 0;
    /* asm zeroes the low half at +0x14 then ANDs the whole word with a mask
       that also clears the (garbage) top byte - net effect: the full
       WldRes.unk14 word ends up zero. */
    req->base.unk14 = 0;

    env->unk0 = 0;
    env->unk4 = 500;
    env->unk6 = 500;
    env->unk8 = 500;
    env->unkA = 0;
    env->unkC = 0;
    env->unkE = 0;
    env->unk10 = 0;
    req->unk1C = env;

    func_801669CC(req, key, arg1, weight);

    *out = req;
}
