#include "../../include/types.h"

extern void func_80167FB8(void); /* 0x168 bytes, calls func_80022A18 (suspicious PSYQ sqrt/vector-length helper) - not yet decompiled */
extern void f_init_80168120_LoadTexInfo(void);

void f_init_80168168_InitTex(void) {
    func_80167FB8();
    f_init_80168120_LoadTexInfo();
}
