#include "../../include/types.h"

extern void **g_main_8011EEFC_ObjArray;

extern s32 func_80080170(void *obj);

/* obj->unk26 (type byte) must be 2, and obj->unk1C->unk0 must have bit 0x4
   set, before func_80080170(obj)'s result is compared against arg1. */
s32 f_main_800B35C4_CheckObjType2Flag(s32 arg0, s32 arg1) {
    void *obj = g_main_8011EEFC_ObjArray[arg0];

    if (*((u8 *) obj + 0x26) != 2)
        return 0;

    if (!(*(s32 *) (*(void **) ((u8 *) obj + 0x1C)) & 4))
        return 0;

    return func_80080170(obj) == (arg1 & 0xFF);
}
