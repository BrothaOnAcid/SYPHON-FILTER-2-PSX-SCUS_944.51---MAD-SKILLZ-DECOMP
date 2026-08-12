#include "../../include/types.h"

extern WldModelHandle *g_main_8011F680_ModelHandles;

/* Toggles bit 0x08 of g_main_8011F680_ModelHandles[modelIndex].res's
   +0x8 flags byte: sets it when `clear` is 0, clears it otherwise. */
void f_main_800AFF0C_SetModelFlag8(s32 clear, s32 modelIndex) {
    WldRes *res = g_main_8011F680_ModelHandles[modelIndex].res;

    if ((clear & 0xFFFF) == 0) {
        res->flags8 |= 0x08;
    } else {
        res->flags8 &= 0xF7;
    }
}
