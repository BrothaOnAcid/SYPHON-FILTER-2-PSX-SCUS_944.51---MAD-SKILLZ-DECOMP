#include "../../include/types.h"

extern s32 D_8011F604;
extern s32 D_8011F580;
extern s32 D_8011F574;
extern s32 D_8011F56C;
extern s32 D_8011F674;
extern s8 *D_8011F688;
extern void *D_8011F5C8;
extern void *D_8011F570;
extern s32 D_80168AE4;

extern void func_800A9FC4(void); /* not yet decompiled */
extern void func_80043CA8(void); /* not yet decompiled */
extern void func_80164650(void); /* not yet decompiled */

/* Guessed 0x5C-stride, 8-entry table - only field +0x0 (word) is touched
   here. */
extern u8 D_80127FF0[];

/* Guessed 0x18-stride, 2-entry table - only field +0x0 (word) is touched
   here. */
extern u8 D_801282D8[];
extern void *D_801282CC;

/* Guessed 0x78-byte level-scratch struct, pointed to by D_8011F570:
     +0x00..+0x24: 10 words, all zeroed except +0x14 = -1
     +0x28: s32[10] (stride 4), all -1
     +0x50: s16[10] (stride 2), all -1
     +0x64: s16[10] (stride 2), all -1
   Parallel byte arrays D_80127D18/D_80127D22/D_80127D2C (10 entries each)
   are cleared alongside the +0x28 array's loop, but live outside this
   struct entirely. */
extern u8 D_80127CA0[];
extern u8 D_80127D18[10];
extern u8 D_80127D22[10];
extern u8 D_80127D2C[10];

/* Guessed second scratch region around D_80127DA0:
     D_80127D98 (base-8): s32[2], all -1
     D_80127DA0 (base+0): s32[4] (two interleaved stride-4 pointers cover
       the same 4 words), all 0
     D_80127DB0 (base+0x10): s16[8] (2 outer * 4 inner, stride 2), all -1
   Parallel byte arrays D_80127DC0/D_80127DC2 (2 entries each). */
extern u8 D_80127DA0[];
extern u8 D_80127DC0[2];
extern u8 D_80127DC2[2];

/* Guessed third scratch region around D_80127D60:
     +0x00: s16[6] (stride 2), all -1
     +0x0C: s16[6] (stride 2), all -1
   Parallel byte array D_80127D78 (6 entries). */
extern u8 D_80127D60[];
extern u8 D_80127D78[6];

/* 26-byte array ending AT D_8012B0AD (i.e. spans D_8012B0AD-0x19..
   D_8012B0AD inclusive), filled with 0x17. */
extern u8 D_8012B0AD;

extern s32 D_80169B98;
extern s32 D_80134E68;
extern s32 D_80134E84;

/* Guessed fourth scratch region around D_8016A3D0:
     +0x00: word, zeroed
     +0x04: s16[8] (stride 2), all -1
     +0x14: s16[8] (stride 2), all -1
   Parallel byte arrays D_8016A3F4/D_8016A3FC (8 entries each). */
extern u8 D_8016A3D0[];
extern u8 D_8016A3F4[8];
extern u8 D_8016A3FC[8];

/* Level-load reset: clears a batch of flags/counters, calls three
   unrelated reset helpers, then bulk-resets several scratch tables to
   their "empty" sentinel values (0 or -1, per-field as documented on each
   extern above). */
void f_init_801591A0_ResetLevelState(void) {
    s32 i;
    s32 *p;

    D_8011F604 = 0;
    D_8011F580 = 0;
    D_8011F574 = 0;
    D_8011F56C = 0;

    func_800A9FC4();
    func_80043CA8();
    func_80164650();

    for (i = 0; i <= 0x284; i += 0x5C) {
        *(s32 *) (D_80127FF0 + i) = 0;
    }

    D_8011F5C8 = &D_801282CC;

    for (i = 0; i <= 0x18; i += 0x18) {
        *(s32 *) (D_801282D8 + i) = 0;
    }

    D_8011F570 = D_80127CA0;
    for (i = 0; i < 10; i++) {
        *(s32 *) (D_80127CA0 + 0x28 + i * 4) = -1;
        *(s16 *) (D_80127CA0 + 0x50 + i * 2) = -1;
        *(s16 *) (D_80127CA0 + 0x64 + i * 2) = -1;
        D_80127D18[i] = 0;
        D_80127D22[i] = 0;
        D_80127D2C[i] = 0;
    }

    for (i = 0; i < 2; i++) {
        s32 j;
        *(s32 *) (D_80127DA0 - 8 + i * 4) = -1;
        *(s32 *) (D_80127DA0 + i * 4) = 0;
        *(s32 *) (D_80127DA0 + 8 + i * 4) = 0;
        for (j = 0; j < 4; j++) {
            *(s16 *) (D_80127DA0 + 0x10 + i * 8 + j * 2) = -1;
        }
        D_80127DC0[i] = 0;
        D_80127DC2[i] = 0;
    }

    for (i = 0; i < 6; i++) {
        *(s16 *) (D_80127D60 + i * 2) = -1;
        *(s16 *) (D_80127D60 + 0xC + i * 2) = -1;
        D_80127D78[i] = 0;
    }

    p = (s32 *) D_80127CA0;
    p[0] = 0;         /* +0x00 */
    p[1] = 0;         /* +0x04 */
    p[2] = 0;         /* +0x08 */
    p[3] = 0;         /* +0x0C */
    p[4] = 0;         /* +0x10 */
    p[5] = -1;        /* +0x14 */
    p[6] = 0;         /* +0x18 */
    p[7] = 0;         /* +0x1C */
    p[8] = 0;         /* +0x20 */
    p[9] = 0;         /* +0x24 */

    for (i = 0; i < 26; i++) {
        (&D_8012B0AD)[-i] = 0x17;
    }

    D_80169B98 = 0;
    D_80134E84 = 0;
    D_80134E68 = 0;
    *(s32 *) D_8016A3D0 = 0;
    for (i = 0; i < 8; i++) {
        *(s16 *) (D_8016A3D0 + 4 + i * 2) = -1;
        *(s16 *) (D_8016A3D0 + 0x14 + i * 2) = -1;
        D_8016A3F4[i] = 0;
        D_8016A3FC[i] = 0;
    }

    D_80168AE4 = 0;
    D_8011F674 = 0;
    *D_8011F688 = 0;
}
