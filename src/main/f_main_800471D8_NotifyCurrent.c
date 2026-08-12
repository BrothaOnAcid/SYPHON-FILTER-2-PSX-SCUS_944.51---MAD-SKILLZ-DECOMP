#include "../../include/types.h"

/* --- not yet decompiled --- */
extern u8 *func_8004714C(void);
extern void func_800419C0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);

/* If func_8004714C returns a "current" object, forwards its
   ->unk8->unk0C field to func_800419C0 with the rest of the args zeroed.
   No-op if there's no current object. */
void f_main_800471D8_NotifyCurrent(void) {
    u8 *cur = func_8004714C();

    if (cur != 0) {
        u8 *sub = *(u8 **) (cur + 8);

        func_800419C0(*(s32 *) (sub + 0xC), 0, 0, 0, 0);
    }
}
