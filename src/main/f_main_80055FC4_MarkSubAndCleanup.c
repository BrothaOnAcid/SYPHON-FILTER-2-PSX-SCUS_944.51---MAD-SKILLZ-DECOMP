#include "../../include/types.h"

/* Sets obj->unk129A = 1 and returns 1 (unconfirmed purpose - marks some
   "done"/"active" flag on the target object). */
extern s32 func_8003584C(u8 *obj);
/* --- not yet decompiled --- */
extern void func_80055EB8(void *arg0);

/* Marks `arg0->unk20`'s target (via func_8003584C), then runs
   func_80055EB8 on `arg0` itself. */
void f_main_80055FC4_MarkSubAndCleanup(u8 *arg0) {
    u8 *sub = *(u8 **) (arg0 + 0x20);

    func_8003584C(*(u8 **) (sub + 0xE0));
    func_80055EB8(arg0);
}
