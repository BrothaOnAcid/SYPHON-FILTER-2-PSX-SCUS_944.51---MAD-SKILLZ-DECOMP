#include "../../include/types.h"

/* --- not yet decompiled --- */
extern void func_80069798(s32 a0);

/* If obj->unkC is set and that object's ->unk158 is non-zero, forwards it
   to func_80069798. No-op otherwise. */
void f_main_800697C4_NotifySubEntry(u8 *obj) {
    u8 *sub = *(u8 **) (obj + 0xC);
    s32 val;

    if (sub == 0) {
        return;
    }
    val = *(s32 *) (sub + 0x158);
    if (val != 0) {
        func_80069798(val);
    }
}
