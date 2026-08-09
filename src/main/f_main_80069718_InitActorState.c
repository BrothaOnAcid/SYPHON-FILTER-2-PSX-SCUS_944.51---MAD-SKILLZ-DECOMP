#include "../../include/types.h"

/* arg1: actor/object being reset into a default idle state; +0x40 is a
   flags word (clears bits 0x4/0x8/0x10/0x20, sets 0xC0). */
void f_main_80069718_InitActorState(void *arg1) {
    u8 *p = (u8 *) arg1;

    *(s32 *) (p + 0x38) = 6;
    *(s32 *) (p + 0x3C) = 6;
    *(u32 *) (p + 0x170) = 0x80000001;
    *(s32 *) (p + 0x8) = 0;
    *(s32 *) (p + 0xC) = 0;
    *(s32 *) (p + 0x10) = 0;
    *(s32 *) (p + 0x18) = 0;
    *(s32 *) (p + 0x1C) = 0;
    *(s32 *) (p + 0x20) = 0;
    *(s32 *) (p + 0x28) = 0;
    *(s32 *) (p + 0x2C) = 0;
    *(s32 *) (p + 0x30) = 0;
    *(s32 *) (p + 0xEC) = 0;
    *(s32 *) (p + 0xF0) = 0;
    *(s32 *) (p + 0x104) = 0;
    *(s32 *) (p + 0x108) = 0;
    *(s32 *) (p + 0x10C) = 0;
    *(u32 *) (p + 0x40) = (*(u32 *) (p + 0x40) & ~0x3C) | 0xC0;
}
