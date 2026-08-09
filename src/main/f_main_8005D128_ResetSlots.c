#include "../../include/types.h"

extern void func_8005689C(void *slot);
extern void func_800569A8(s32 a0, void *slot);
extern void func_80056EC4(s32 a0);

/* Walks 7 stride-0xC slots starting at arg0+0xA28+0xFC, re-registering
   each with arg0->unk1C, releasing its own +0x8 field, then finalizing it. */
void f_main_8005D128_ResetSlots(void *arg0) {
    u8 *base = (u8 *) arg0 + 0xA28;
    s32 i;
    u8 *slot;

    for (i = 0; i < 7; i++) {
        slot = base + 0xFC + i * 0xC;
        func_800569A8(*(s32 *) ((u8 *) arg0 + 0x1C), slot);
        func_80056EC4(*(s32 *) (slot + 0x8));
        func_8005689C(slot);
    }
}
