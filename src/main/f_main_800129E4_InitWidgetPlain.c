#include "../../include/types.h"

/* Sibling of f_main_800129A0_InitWidget: same 0x24-stride widget struct,
   but tags +0x8 as 0x05000000 and +0xC style as style|0x28000000, and only
   fills 4 color slots (no +0x20). Called from f_init_801651E8_BuildMenuWidgets
   region (init/0.s 0x801656EC etc) with matching stack-passed args. */
void f_main_800129E4_InitWidgetPlain(void *w, s32 style, s32 a2, s32 a3, s32 a5, s32 a6) {
    u8 *p = (u8 *) w;

    *(s32 *) (p + 8) = 0x05000000;
    *(s16 *) (p + 4) = 0;
    *(s32 *) (p + 0xC) = style | 0x28000000;
    *(s32 *) (p + 0x10) = a2;
    *(s32 *) (p + 0x14) = a3;
    *(s16 *) (p + 6) = 4;
    *(s32 *) (p + 0x18) = a5;
    *(s32 *) (p + 0x1C) = a6;
}
