#include "../../include/types.h"

extern void *f_main_80025AD0_AllocDown(s32 size);
extern void func_800B9A58(void *arg0);                          /* not yet decompiled */
extern void f_init_8016648C_ResetThenSetIndex(s32 arg0);         /* really takes arg0 as a pointer, see that file */
extern void func_8016327C(void *arg0, s8 *mode);                 /* not yet decompiled */
extern s32 f_init_8016357C_AllocPool18(void **out, s32 tmpl, s32 owner, s32 base);
extern void func_801636D0(s32 id, s32 index, s32 *out);           /* not yet decompiled */
extern void func_80163680(void *arg0, s32 arg1);                  /* not yet decompiled */
extern void func_8008F178(void *arg0, s32 arg1);                   /* not yet decompiled */

/* guess: 12-field "entity template" descriptor, built on the stack and fed
   to f_init_8016357C_AllocPool18 -> f_init_801633DC_InitEntityFromTemplate
   (whose `src` param is exactly this layout). */
typedef struct {
    s32 unk0;   /* +0x00: varies per call, see below */
    s32 unk4;   /* +0x04: always 0 */
    s32 unk8;   /* +0x08: varies per call */
    s32 unkC;   /* +0x0C: varies per call */
    u8 _pad10[0x4]; /* +0x10: NOTE - never initialized here, garbage stack
                        bytes; f_init_801633DC_InitEntityFromTemplate still
                        copies it verbatim */
    s16 unk14;  /* +0x14: varies per call */
    s16 unk16;  /* +0x16: always -0x7FFF */
    s16 unk18;  /* +0x18: always 0x7FFF */
    s16 unk1A;  /* +0x1A: always 0xB50 */
    s16 unk1C;  /* +0x1C: always 0x7FFF */
    s32 unk20;  /* +0x20: always 0 */
    s32 unk24;  /* +0x24: always 0 */
    s32 unk28;  /* +0x28: always 0x1000 */
    s32 unk2C;  /* +0x2C: always 0x1000 */
} EntityTemplate;

/* Heavily unconfirmed - `arg0` shares the arg0->unk20->unk108 buffer chain
   already seen in f_init_80166158_BuildPatchGrid / f_init_8016648C.
   Allocates a 0x45E8-byte buffer at arg0->unk20->unk108, runs
   func_800B9A58/f_init_8016648C_ResetThenSetIndex/func_8016327C setup on
   `arg0`, then builds 4 EntityTemplate descriptors (differing only in
   unk0/unk8/unkC/unk14) and hands each to f_init_8016357C_AllocPool18,
   filling the buffer's first 4 pointer slots (+0x0/+0x4/+0x8/+0xC).
   Finishes by resetting arg0->unkC->unk160's bbox-ish fields from the
   buffer's unk0 pointer, feeding its unk4/unk8/unkC pointers through
   func_801636D0 into func_80163680, calling func_8008F178(arg0, -1), then
   stamping a couple of fixed values into arg0->unkC. Return value (if any)
   is unused by the one known caller. */
void f_init_8016372C_InitEntityPools(void *arg0) {
    u8 *a = (u8 *) arg0;
    void *buf = f_main_80025AD0_AllocDown(0x45E8);
    void *coreA = *(void **) (a + 0x20);
    void *core = *(void **) (a + 0xC);
    void *def = *(void **) (a + 0x8);
    void *defB;
    s8 mode = 3;
    EntityTemplate tpl;
    s32 scratch; /* reused/overwritten by each func_801636D0 call below */

    *(void **) ((u8 *) coreA + 0x108) = buf;

    func_800B9A58(arg0);
    f_init_8016648C_ResetThenSetIndex((s32) (u64) arg0);
    func_8016327C(arg0, &mode);

    *(u8 *) ((u8 *) core + 0x102) = 1;
    defB = *(void **) ((u8 *) def + 0x18);

    tpl.unk4 = 0;
    tpl.unk16 = -0x7FFF;
    tpl.unk18 = 0x7FFF;
    tpl.unk1A = 0xB50;
    tpl.unk1C = 0x7FFF;
    tpl.unk20 = 0;
    tpl.unk24 = 0;
    tpl.unk28 = 0x1000;
    tpl.unk2C = 0x1000;

    tpl.unk8 = -0x39;
    tpl.unkC = 0;
    tpl.unk14 = 0x30;
    tpl.unk0 = *(s32 *) ((u8 *) def + 0xC);
    f_init_8016357C_AllocPool18((void **) buf, (s32) (u64) &tpl, (s32) (u64) arg0, (s32) (u64) ((u8 *) buf + 0x10));

    tpl.unk8 = -4;
    tpl.unkC = 0;
    tpl.unk14 = 0x14;
    tpl.unk0 = *(s32 *) ((u8 *) defB + 0x38);
    f_init_8016357C_AllocPool18((void **) ((u8 *) buf + 0xC), (s32) (u64) &tpl, (s32) (u64) arg0, (s32) (u64) ((u8 *) buf + 0x10));

    tpl.unk8 = 3;
    tpl.unkC = -5;
    tpl.unk14 = 0x10;
    tpl.unk0 = *(s32 *) ((u8 *) defB + 0x1C);
    f_init_8016357C_AllocPool18((void **) ((u8 *) buf + 4), (s32) (u64) &tpl, (s32) (u64) arg0, (s32) (u64) ((u8 *) buf + 0x10));

    tpl.unk8 = 3;
    tpl.unkC = -5;
    tpl.unk14 = 0x10;
    tpl.unk0 = *(s32 *) ((u8 *) defB + 0x4);
    f_init_8016357C_AllocPool18((void **) ((u8 *) buf + 8), (s32) (u64) &tpl, (s32) (u64) arg0, (s32) (u64) ((u8 *) buf + 0x10));

    {
        void *bbox = *(void **) ((u8 *) core + 0x160);
        s32 origin = *(s32 *) buf;

        *(s32 *) ((u8 *) bbox + 0x130) = 0;
        *(s32 *) ((u8 *) bbox + 0x128) = origin;
        *(s32 *) ((u8 *) bbox + 0x12C) = origin;
    }

    scratch = 0;
    func_801636D0(*(s32 *) ((u8 *) buf + 4), 0, &scratch);
    func_801636D0(*(s32 *) ((u8 *) buf + 8), 1, &scratch);
    func_801636D0(*(s32 *) ((u8 *) buf + 0xC), 2, &scratch);
    func_80163680(arg0, scratch);

    func_8008F178(arg0, -1);

    *(u8 *) (*(void **) ((u8 *) core + 0x160)) = 3;
    *(s32 *) ((u8 *) (*(void **) ((u8 *) core + 0x158)) + 4) = 0x599;
}
