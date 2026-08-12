#include "../../include/types.h"

/* --- not yet decompiled --- */
extern void func_80093530(void);
extern void func_80093CB4(void);

/* Two-way dispatch on obj->unk26 (state id): state 9 goes to
   func_80093CB4, everything else to func_80093530. */
void f_main_80093AE0_DispatchByState9(u8 *obj) {
    if (obj[0x26] == 9) {
        func_80093CB4();
    } else {
        func_80093530();
    }
}
