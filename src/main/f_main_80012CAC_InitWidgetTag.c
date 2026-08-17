#include "../../include/types.h"

/* Another sibling of f_main_800129A0_InitWidget on the same 0x24-stride
   widget struct: tags +0x8 as 0x02000000, style +0xC as style|0x68000000,
   only fills the +0x10 color slot. Called from around 0x8006CB0C with a
   struct-array element (stride 0x20) + 4. */
void f_main_80012CAC_InitWidgetTag(void *w, s32 style, s32 a2) {
    u8 *p = (u8 *) w;

    *(s16 *) (p + 6) = 4;
    *(s32 *) (p + 8) = 0x02000000;
    *(s16 *) (p + 4) = 0;
    *(s32 *) (p + 0xC) = style | 0x68000000;
    *(s32 *) (p + 0x10) = a2;
}
