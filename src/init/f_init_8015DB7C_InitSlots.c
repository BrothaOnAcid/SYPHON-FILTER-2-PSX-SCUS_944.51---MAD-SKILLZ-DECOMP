#include "../../include/types.h"

/* Guessed 0x5C-byte record array (8 entries at g_init_80127FEC_Slots);
   only +0x08 and +0x48 are referenced here (passed by address). */
typedef struct {
    u8 _pad00[0x5C];
} SlotRecord;

/* Guessed created-object type; only +0x5/+0x6 (bytes) are confirmed. */
typedef struct {
    u8 _pad00[0x5];
    u8 unk05;
    u8 unk06;
} SlotObj;

extern SlotRecord g_init_80127FEC_Slots[8];
extern s32 g_init_80127FF0_Counts[8];      /* parallel to Slots, stride 0x5C */
extern u8 g_init_80128044_ByteA[8];
extern u8 g_init_80128045_ByteB[8];

/* --- libc-style externs (not yet decompiled) --- */
extern SlotObj *func_80016A0C(SlotRecord *slot, void *field8, s32 count,
                               void *field48, s32 extra);

/* For each of 8 slots: bail out entirely (not just skip) the first time
   g_init_80127FF0_Counts[i] <= 0; otherwise create/register the slot via
   func_80016A0C and tag the result with two per-slot config bytes. */
void f_init_8015DB7C_InitSlots(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        SlotRecord *slot = &g_init_80127FEC_Slots[i];
        SlotObj *obj;

        if (g_init_80127FF0_Counts[i] <= 0)
            return;

        obj = func_80016A0C(slot, (u8 *) slot + 0x8,
                             g_init_80127FF0_Counts[i],
                             (u8 *) slot + 0x48, 0);
        obj->unk06 = g_init_80128045_ByteB[i];
        obj->unk05 = g_init_80128044_ByteA[i];
    }
}
