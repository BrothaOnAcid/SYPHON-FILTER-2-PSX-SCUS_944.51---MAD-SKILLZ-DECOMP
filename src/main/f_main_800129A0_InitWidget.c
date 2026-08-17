#include "../../include/types.h"

/* Widget field layout (stride 0x24, see f_init_801651E8_BuildMenuWidgets):
   +0x4 group tag, +0x6 kind, +0x8 flags, +0xC style|0x30000000,
   +0x10/+0x14/+0x18/+0x1C colors, +0x20 color */
void f_main_800129A0_InitWidget(void *w, s32 style, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6) {
    u8 *p = (u8 *) w;

    *(s16 *) (p + 6) = 4;
    *(s32 *) (p + 8) = 0x06000000;
    *(s32 *) (p + 4) = 0;
    *(s32 *) (p + 0xC) = style | 0x30000000;
    *(s32 *) (p + 0x10) = a2;
    *(s32 *) (p + 0x18) = a3;
    *(s32 *) (p + 0x20) = a4;
    *(s32 *) (p + 0x14) = a5;
    *(s32 *) (p + 0x1C) = a6;
}
