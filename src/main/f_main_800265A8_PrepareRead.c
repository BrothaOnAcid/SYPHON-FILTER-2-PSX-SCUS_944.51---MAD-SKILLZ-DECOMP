#include "../../include/types.h"

extern u8 g_main_8011EE50_CdStreamActive;

extern void func_80026910(s32 flag);
extern s32 func_8002619C(FileHandle *h);

/* Validates the handle, then seeds the read-cursor fields (+0xC = handle
   id, +0x10 = size) from the base fields before starting the read. */
s32 f_main_800265A8_PrepareRead(FileHandle *h) {
    if (h == 0)
        return 1;

    if (h->mode < 0)
        return 4;

    if (g_main_8011EE50_CdStreamActive == 1)
        func_80026910(0);

    h->unk0C = h->unk00;
    h->unk10 = h->size;

    return func_8002619C(h);
}
