#include "../../include/types.h"

extern void func_80166158(void);
extern void func_801663E0(s32 index);

void f_init_8016648C_ResetThenSetIndex(s32 arg0) {
    func_80166158();
    func_801663E0(arg0);
}
