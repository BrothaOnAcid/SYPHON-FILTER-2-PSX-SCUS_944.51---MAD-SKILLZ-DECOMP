#include "../../include/types.h"

extern s8 g_main_8011F628_unk;

extern void func_8002BE68(void); /* not yet decompiled, 0x254 bytes */

void f_main_8002BE3C_TriggerFlag8068(void) {
    g_main_8011F628_unk = 1;
    func_8002BE68();
}
