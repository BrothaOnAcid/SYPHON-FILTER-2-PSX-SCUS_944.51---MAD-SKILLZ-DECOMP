#include "../../include/types.h"

extern u8 g_main_8011EE3E_unk;

s32 f_main_80025F58_SetFlag(s32 val) {
    g_main_8011EE3E_unk = (u8) val;
    return 0;
}
