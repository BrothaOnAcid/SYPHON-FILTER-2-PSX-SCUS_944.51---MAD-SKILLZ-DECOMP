#include "../../include/types.h"

extern u16 D_8011EC70;   /* not yet decompiled: spawn priority threshold, see f_init_80167510_SpawnEntity */
extern s16 D_8011F4E8;   /* not yet decompiled */
extern s32 D_8011ECFC;   /* not yet decompiled */
extern s32 D_8011ECA8;   /* not yet decompiled */
extern s32 D_8011EC90;   /* not yet decompiled */
extern s32 D_8011EC94;   /* not yet decompiled */
extern u8 D_8011EC98;    /* not yet decompiled */
extern s32 D_8011EC9C;   /* not yet decompiled */
extern s32 D_8011ECA4;   /* not yet decompiled */
extern s32 D_8011ED08;   /* not yet decompiled */
extern u8 D_8011EC99;    /* not yet decompiled */
extern u8 D_8011EC9A;    /* not yet decompiled */
extern s32 D_8011ED98;   /* not yet decompiled */
extern s32 D_8011EC74;   /* not yet decompiled */
extern s32 D_8011ECF4;   /* not yet decompiled */
extern s16 D_8011ED0E;   /* not yet decompiled */
extern u8 D_80121260[];  /* not yet decompiled: array of 12 elements, stride 0xC, only field +0 is cleared here */

extern void func_800197A8(void); /* not yet decompiled */
extern void f_main_80016D7C_ClearSlotTable(void);

/* guess: resets the entity-spawn subsystem's globals to their empty state
   (spawn priority threshold, several counters/flags, a 12-entry table at
   D_80121260 with stride 0xC), then runs two more subsystem resets via
   func_800197A8/f_main_80016D7C_ClearSlotTable. Splat merged this with the following
   function into one nonmatching block (like the documented boundary bug
   at 0x80168298); the real prologue for the next function starts right
   after this one's epilogue, at 0x80166E54. */
void f_init_80166D8C_ResetEntitySpawnState(void) {
    s32 i;

    D_8011EC70 = 0;
    D_8011F4E8 = 0;
    D_8011ECFC = 0;
    D_8011ECA8 = 0;
    D_8011EC90 = 0;
    D_8011EC94 = 0;
    D_8011EC98 = 1;
    D_8011EC9C = 0;
    D_8011ECA4 = 0;
    D_8011ED08 = 0;
    D_8011EC99 = 0;
    D_8011EC9A = 0;
    D_8011ED98 = 0;
    D_8011EC74 = 0;

    for (i = 0; i < 12; i++) {
        *(s32 *) (D_80121260 + i * 0xC) = 0;
    }

    D_8011ECF4 = 0;
    D_8011ED0E = -1;

    func_800197A8();
    f_main_80016D7C_ClearSlotTable();
}
