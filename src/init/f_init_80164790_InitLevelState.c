#include "../../include/types.h"

extern void *f_main_80025AD0_AllocDown(s32 size);
extern void func_8001755C(void *arg0);              /* not yet decompiled */
extern void func_80024000(void *arg0);              /* not yet decompiled */
extern void func_80017570(void *arg0, s32 arg1);     /* not yet decompiled */
extern void func_80017578(void *arg0, s32 arg1, s32 arg2); /* not yet decompiled */
extern void func_8002AC88(s32 a0, s32 a1, s32 a2, s32 a3, s32 sp10, s32 sp14, s32 sp18, s32 sp1C); /* not yet decompiled */
extern s16 func_800F64D4(s32 arg0);                  /* not yet decompiled */
extern void func_80075FF4(void *arg0);               /* not yet decompiled */

extern u8 D_8011F624;   /* not yet decompiled */
extern void *D_8011F80C; /* not yet decompiled: 0x28-entry array, stride 0x3C, see SlotEntry */
extern s16 D_8011F820;   /* not yet decompiled: entry count for D_8011F80C, fixed to 0x28 here */
extern u8 *D_8011F7F0;   /* not yet decompiled: (D_8011F820+1)-byte array */
extern void *D_8011F7F4; /* not yet decompiled: 0x2D00-byte buffer, only allocated when D_8011F624 != 0 */

extern s32 D_8012FCBC; /* not yet decompiled: 0x78-entry array, stride 0x74, see LinkEntry */

extern u8 D_80133350[]; /* not yet decompiled: 2-entry array, stride 0x34 */
extern u8 D_8013337C[]; /* not yet decompiled: 2-entry array, stride 0x34 (only +0x0 read here), arg1 source for func_80017570 */

extern s16 D_8013330C[]; /* not yet decompiled: 0x20-entry s16 array, filled from func_800F64D4 */

extern s32 D_8011F7BC;
extern s16 D_8011F7C0;
extern s16 D_8011F7C4;
extern s32 D_8011F7C8;
extern s16 D_8011F7D0;
extern s32 D_8011F7D4;
extern s32 D_8011F7DC;
extern s8 D_8011F7E0;
extern s8 D_8011F7E4;
extern s8 D_8011F7E8;
extern s16 D_8011F7EC;
extern s32 D_8011F804;
extern s16 D_8011F800;
extern s16 D_8011F808;
extern s32 D_8011F814;
extern s32 D_8011F818;
extern s8 D_8011F828;
extern s8 D_8011F82C;
extern s8 D_8011F830;
extern s16 D_8011F834;
extern s16 D_8011F83C;
extern s32 D_801332FC;
extern s32 D_80133300;
extern s32 D_80133304;

/* guess: entry in D_8011F80C, stride 0x3C. Only these 8 fields are set by
   f_init_80164790_InitLevelState; the rest of the 0x3C bytes are untouched. */
typedef struct {
    s32 unk0;   /* +0x00 */
    u8 _pad4[0x1C];
    s16 unk20;  /* +0x20 */
    u8 _pad22[0x6];
    s16 unk28;  /* +0x28 */
    u8 _pad2A[0x2];
    u8 unk2C;   /* +0x2C */
    u8 unk2D;   /* +0x2D */
    u8 unk2E;   /* +0x2E */
    u8 unk2F;   /* +0x2F */
    u8 unk30;   /* +0x30 */
} SlotEntry;

/* guess: entry in D_8012FCBC, stride 0x74. */
typedef struct {
    s32 flags0; /* +0x00: bits 0x06000000 cleared here */
    u8 _pad4[0x2];
    s16 unk6;   /* +0x06: reset to -0x8000 */
} LinkEntry;

/* Heavily unconfirmed level-state init: allocates D_8011F80C (0x28
   SlotEntry-sized slots), a companion status byte array D_8011F7F0
   (initialized to 0xFF), and (only when D_8011F624 is set) a 0x2D00-byte
   scratch buffer D_8011F7F4. Resets every LinkEntry in D_8012FCBC. Resets
   ~20 scattered globals to 0, runs a 2-iteration setup pass over
   D_80133350 (func_8001755C/func_80024000/func_80017570/func_80017578),
   calls func_8002AC88 with fixed params, fills a 0x20-entry lookup table
   D_8013330C via func_800F64D4, and finally calls func_80075FF4 on
   D_8011F80C. */
void f_init_80164790_InitLevelState(void) {
    SlotEntry *slots;
    LinkEntry *links;
    u8 *dataA;
    u8 *dataB;
    s32 i;

    D_8011F820 = 0x28;
    D_8011F80C = f_main_80025AD0_AllocDown(0x960);
    D_8011F7F0 = (u8 *) f_main_80025AD0_AllocDown(D_8011F820 + 1);

    if (D_8011F624 != 0) {
        D_8011F7F4 = f_main_80025AD0_AllocDown(0x2D00);
    }

    slots = (SlotEntry *) D_8011F80C;
    for (i = 0; i < D_8011F820; i++) {
        slots[i].unk2C = 0xFF;
        slots[i].unk2F = 0;
        slots[i].unk2D = 0;
        slots[i].unk2E = 0;
        slots[i].unk30 = (u8) i;
        slots[i].unk0 = 0;
        slots[i].unk28 = -1;
        slots[i].unk20 = 0;
        D_8011F7F0[i] = 0xFF;
    }
    D_8011F7F0[D_8011F820] = 0xFF;

    D_8011F834 = 0;
    D_8011F800 = 0;

    links = (LinkEntry *) &D_8012FCBC;
    for (i = 0; i < 0x78; i++) {
        links[i].unk6 = -0x8000;
        links[i].flags0 &= 0xF9FFFFFF;
    }

    D_8011F7C0 = 0;
    D_8011F7EC = 0;
    D_8011F808 = 0;
    D_8011F7D0 = 0;
    D_8011F7C8 = 0x80000001;
    D_8011F83C = 0;
    D_8011F7E4 = 0;
    D_8011F818 = 0;
    D_8011F814 = 0;
    D_8011F7C4 = 0;
    D_8011F7BC = 0;
    D_8011F7DC = 0;
    D_8011F804 = 0;
    D_8011F830 = 0;
    D_8011F82C = 0;
    D_8011F828 = 0;
    D_80133304 = 0;
    D_80133300 = 0;
    D_801332FC = 0;
    D_8011F7E8 = 0;
    D_8011F7D4 = 0;
    D_8011F7E0 = 0;

    dataA = D_80133350;
    dataB = dataA + 0x2C;
    for (i = 0; i < 2; i++) {
        func_8001755C(dataA);
        func_80024000(dataB);
        func_80017570(dataA, *(s32 *) (D_8013337C + i * 0x34));
        func_80017578(dataA, 0x64, 0x20FFFFFF);
        dataA += 0x34;
        dataB += 0x34;
    }

    func_8002AC88(0xE, 4, 0xFFFE, 0xFFFE, 0, 0, 0, 0);

    for (i = 0; i < 0x20; i++) {
        D_8013330C[i] = func_800F64D4(i << 7);
    }

    func_80075FF4(D_8011F80C);
}
