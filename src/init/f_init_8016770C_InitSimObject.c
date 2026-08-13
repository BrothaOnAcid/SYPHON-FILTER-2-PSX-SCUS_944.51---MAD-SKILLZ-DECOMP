#include "../../include/types.h"

extern void func_8002E838(void *obj, u8 flag);                     /* not yet decompiled */
extern void func_8002E93C(void *obj, u8 flag);                     /* not yet decompiled */
extern void func_8002E988(void *obj, s32 arg1, s32 **arg2);         /* not yet decompiled */
extern void func_8002EFF0(void *obj, s32 index, s32 arg2, s32 arg3); /* not yet decompiled */
extern void *f_main_800F4190_memset(void *dst, s32 fillValue, s32 size);
extern s32 *func_80167510(s32 *pos, s32 *vel, s32 arg2, s32 arg3, s32 arg4, s32 arg5); /* not yet decompiled */

extern s32 D_8011F5CC; /* not yet decompiled: running count, indexes D_801283AC (stride 0x8C) */
extern s32 D_8011F5DC; /* not yet decompiled: running count, indexes D_801285DC (stride 0x144) */
extern u8 D_801283AC[]; /* not yet decompiled: table, stride 0x8C */
extern u8 D_801285DC[]; /* not yet decompiled: table, stride 0x144 */
extern s32 D_80158BEC[4]; /* all-zero default vector/quad constant, see f_init_801633DC_InitEntityFromTemplate */

/* guess: 0xDC-byte context object built by f_init_8016770C_InitSimObject.
   unk0 is a shared 4-word vector (pos?); unkC caches its first word, and
   unk10..1C separately cache `pos`'s 4 words. unk40..4C cache `vel`'s 4
   words. unk98/unkA0 are the per-call counts passed in, seeding running
   totals D_8011F5CC/D_8011F5DC and offsets into D_801283AC/D_801285DC
   stashed at unk9C/unkA4. Everything else set here (unk20..38, unk50..68)
   is just zeroed. */
typedef struct {
    s32 *unk0;   /* +0x00 */
    u8 _pad4[0x8];
    s32 unkC;    /* +0x0C */
    s32 unk10;   /* +0x10 */
    s32 unk14;   /* +0x14 */
    s32 unk18;   /* +0x18 */
    s32 unk1C;   /* +0x1C */
    s32 unk20;   /* +0x20 */
    s32 unk24;   /* +0x24 */
    s32 unk28;   /* +0x28 */
    u8 _pad2C[0x4];
    s32 unk30;   /* +0x30 */
    s32 unk34;   /* +0x34 */
    s32 unk38;   /* +0x38 */
    u8 _pad3C[0x4];
    s32 unk40;   /* +0x40 */
    s32 unk44;   /* +0x44 */
    s32 unk48;   /* +0x48 */
    s32 unk4C;   /* +0x4C */
    s32 unk50;   /* +0x50 */
    s32 unk54;   /* +0x54 */
    s32 unk58;   /* +0x58 */
    u8 _pad5C[0x4];
    s32 unk60;   /* +0x60 */
    s32 unk64;   /* +0x64 */
    s32 unk68;   /* +0x68 */
    u8 _pad6C[0x2C];
    s32 unk98;   /* +0x98 */
    void *unk9C; /* +0x9C */
    s32 unkA0;   /* +0xA0 */
    void *unkA4; /* +0xA4 */
} SimObj;

/* Heavily unconfirmed. Zeroes *obj (0xDC bytes) then fills it: unk0 either
   from `vec` directly or (if NULL) from f_init_80167510-ish lookup based on
   `pos`/`vel`/`life`/`count`/`mode`/`arg7`; unkC/10..1C/40..4C cached from
   *unk0 and `pos`/`vel` (both default to the zero vector D_80158BEC when
   NULL); unk98/unkA0 seed running totals D_8011F5CC/D_8011F5DC. Runs
   func_8002EFF0 once per unk98 entry, then func_8002E988/func_8002E838
   (only if flag1 == 1)/func_8002E93C. Returns 0 (no-op) if obj is NULL or
   count == 0, else 1. */
u8 f_init_8016770C_InitSimObject(SimObj *obj, s32 *vec, s32 *pos, s32 *vel, s32 life,
                                  s32 count, u8 mode, s32 arg7, s32 numA, s32 numB,
                                  s32 arg10, s32 **linkOut, u8 flag1, u8 flag2) {
    s32 zero[4];
    s32 zeroPos[4];
    s32 *nullLink;

    zero[0] = D_80158BEC[0];
    zero[1] = D_80158BEC[1];
    zero[2] = D_80158BEC[2];
    zero[3] = D_80158BEC[3];
    zeroPos[0] = D_80158BEC[0];
    zeroPos[1] = D_80158BEC[1];
    zeroPos[2] = D_80158BEC[2];
    zeroPos[3] = D_80158BEC[3];

    nullLink = (s32 *) -1; /* asm also zeroes this stack slot first via
                               f_main_800F4190_memset(&nullLink, 0, 8), but that's
                               immediately overwritten and never read
                               otherwise - dropped as a no-op */

    if (obj == 0 || count == 0) {
        return 0;
    }

    f_main_800F4190_memset(obj, 0, 0xDC);
    obj->unk98 = numA;
    obj->unk9C = D_801283AC + D_8011F5CC * 0x8C;
    obj->unkA0 = numB;
    obj->unkA4 = D_801285DC + D_8011F5DC * 0x144;
    D_8011F5CC += numA;
    D_8011F5DC += numB;

    if (pos == 0) {
        pos = zero;
    }
    if (life < 0) {
        life = 0x2AA;
    }
    if ((mode & 0xFF) >= 5) {
        mode = 2;
    }
    if (vel == 0) {
        vel = zeroPos;
    }
    if (linkOut == 0) {
        linkOut = &nullLink;
    }

    if (vec == 0) {
        obj->unk0 = func_80167510(pos, vel, life, count, mode & 0xFF, arg7);
    } else {
        obj->unk0 = vec;
    }

    obj->unkC = *obj->unk0;
    obj->unk10 = pos[0];
    obj->unk14 = pos[1];
    obj->unk18 = pos[2];
    obj->unk1C = pos[3];
    obj->unk20 = 0;
    obj->unk24 = 0;
    obj->unk28 = 0;
    obj->unk30 = 0;
    obj->unk34 = 0;
    obj->unk38 = 0;
    obj->unk40 = vel[0];
    obj->unk44 = vel[1];
    obj->unk48 = vel[2];
    obj->unk4C = vel[3];
    obj->unk50 = 0;
    obj->unk54 = 0;
    obj->unk58 = 0;
    obj->unk60 = 0;
    obj->unk64 = 0;
    obj->unk68 = 0;

    {
        s32 i;
        for (i = 0; i < obj->unk98; i++) {
            func_8002EFF0(obj, i, 0, 0);
        }
    }

    func_8002E988(obj, arg10, linkOut);

    if (flag1 == 1) {
        func_8002E838(obj, 1);
    }
    func_8002E93C(obj, flag2);

    return 1;
}
