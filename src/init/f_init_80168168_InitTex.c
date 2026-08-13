#include "../../include/types.h"

extern void f_init_80167FB8_BuildShadeConeTable(void);
extern void f_init_80168120_LoadTexInfo(void);

void f_init_80168168_InitTex(void) {
    f_init_80167FB8_BuildShadeConeTable();
    f_init_80168120_LoadTexInfo();
}
