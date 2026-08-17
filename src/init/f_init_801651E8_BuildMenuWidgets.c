#include "../../include/types.h"

extern void *f_main_80025AD0_AllocDown(s32 size);
extern void func_800128A4(s32 group, void *widget);              /* not yet decompiled: register widget (type A) */
extern void func_800128D8(s32 group, void *widget);              /* not yet decompiled: register widget (type B) */
extern void f_main_800129A0_InitWidget(void *widget, s32 style, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);
extern void func_80012BF8(void *widget, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8); /* not yet decompiled */
extern void func_80012C80(void *widget, s32 a1, s32 a2, s32 a3);  /* not yet decompiled */
extern void func_800A5AFC(void *widget, s32 arg1);                /* not yet decompiled */
extern s32 func_800A7038(void *font, void *widget, s32 arg2, s32 arg3); /* not yet decompiled */
extern void func_800A7430(s32 id, s32 flag);                       /* not yet decompiled */
extern s32 func_800A8068(s32 id, void *arg1);                      /* not yet decompiled */

extern void *D_80114A74;   /* not yet decompiled: menu widget block, set here */
extern u8 D_8011EC89;      /* not yet decompiled: region/language flag? gates several layouts below */
extern s32 D_8011F9C4;     /* not yet decompiled: widget group id passed to func_800128A4/func_800128D8 */
extern void *D_80158BC8;   /* not yet decompiled: font/style table */
extern void *D_80158C64;   /* not yet decompiled: font/style table */
extern void *D_80158C68;   /* not yet decompiled: font/style table (only used by dead code below) */

/* Heavily unconfirmed menu/HUD widget setup: allocates a 0x3C4-byte block
   (stored at D_80114A74) and builds four groups of fixed-layout widgets
   inside it via f_main_800129A0_InitWidget (6x, stride 0x24, sets a1 as the
   "style" field OR'd with 0x30000000)/func_80012BF8 (4x, stride
   0x2C)/func_80012C80 (2x, stride 0x18, then 12x, stride 0x18), each
   registered via func_800128A4 and tagged (widget->unk4 = 1 or 2). Then
   sets up a couple of text/label handles (unk2D8/unk2DC) via
   func_800A5AFC/func_800A7038/func_800A7430/func_800A8068, gated on
   D_8011EC89. Finally re-registers three of the widget groups via
   func_800128D8 instead and sets unk2E0 to -1.

   The middle block (unk2E0 via D_80158C68) is dead code - it's guarded by
   a local flag that's hardcoded to 0 at function entry and never modified,
   so the branch around it is always taken. Kept for fidelity. */
void f_init_801651E8_BuildMenuWidgets(void) {
    u8 *base = (u8 *) f_main_80025AD0_AllocDown(0x3C4);
    s32 deadFlag = 0;
    s32 i;
    s32 off;
    s32 id2D8;
    void *w;

    D_80114A74 = base;

    off = 0x150;
    for (i = 0; i < 6; i++) {
        w = base + off;
        f_main_800129A0_InitWidget(w, 0, 0x04000400, 0x04000400, 0x04000400, 0x503028, 0x503028);
        off += 0x24;
        func_800128A4(D_8011F9C4, w);
        *(s32 *) ((u8 *) w + 4) = 1;
    }

    off = 0x228;
    for (i = 0; i < 4; i++) {
        w = base + off;
        func_80012BF8(w, 0, 0x04000400, 0x04000400, 0x04000400, 0x04000400, 0, 0x503028, 0x503028);
        off += 0x2C;
        func_800128A4(D_8011F9C4, w);
        *(s32 *) ((u8 *) w + 4) = 1;
    }

    w = base;
    for (i = 0; i < 2; i++) {
        func_80012C80(w, 0x503028, 0x04000400, 0x04000400);
        func_800128A4(D_8011F9C4, w);
        *(s32 *) ((u8 *) w + 4) = 2;
        w = (u8 *) w + 0x18;
    }

    off = 0x30;
    for (i = 0; i < 0xC; i++) {
        w = base + off;
        func_80012C80(w, 0x78463C, 0x04000400, 0x04000400);
        off += 0x18;
        func_800128A4(D_8011F9C4, w);
        *(s32 *) ((u8 *) w + 4) = 2;
    }

    w = base + 0x2E4;
    func_800A5AFC(w, 0xE);

    if (D_8011EC89 != 0) {
        id2D8 = func_800A7038(&D_80158C64, w, -9, -0x69);
    } else {
        id2D8 = func_800A7038(&D_80158C64, w, -0x93, -8);
    }
    func_800A7430(id2D8, 1);
    *(s32 *) (base + 0x2D8) = func_800A8068(id2D8, &D_80158BC8);

    if (D_8011EC89 != 0) {
        *(s32 *) (base + 0x2DC) = func_800A8068(
            func_800A7038(&D_80158C64, base + 0x314, 8, 0x61), &D_80158BC8);
    } else {
        *(s32 *) (base + 0x2DC) = func_800A8068(
            func_800A7038(&D_80158C64, base + 0x314, 0x93, 0), &D_80158BC8);
    }

    if (deadFlag != 0) {
        s32 id2E0 = func_800A8068(func_800A7038(&D_80158C68, base + 0x344, 0, -4), &D_80158BC8);
        *(s32 *) (base + 0x2E0) = id2E0;
        func_800A7430(id2E0, 0);
        return;
    }

    off = 0x198;
    for (i = 2; i < 6; i++) {
        func_800128D8(D_8011F9C4, base + off);
        off += 0x24;
    }

    off = 0x280;
    for (i = 2; i < 4; i++) {
        func_800128D8(D_8011F9C4, base + off);
        off += 0x2C;
    }

    off = 0xC0;
    for (i = 6; i < 0xC; i++) {
        func_800128D8(D_8011F9C4, base + off);
        off += 0x18;
    }

    *(s32 *) (base + 0x2E0) = -1;
}
