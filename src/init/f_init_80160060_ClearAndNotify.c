#include "../../include/types.h"

extern void func_8015FFD0(ObjRecord *arg0, s32 cmd); /* not yet decompiled */

void f_init_80160060_ClearAndNotify(ObjRecord *arg0) {
    arg0->unk25 = 0xFF;
    func_8015FFD0(arg0, 0x2E);
    func_8015FFD0(arg0, 0x19);
}
