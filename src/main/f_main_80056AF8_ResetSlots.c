#include "../../include/types.h"

/* guess: zeroes/reinitializes `count` slots starting at `slots` to their
   default state (see ResetSlot). */
void f_main_80056AF8_ResetSlots(ResetSlot *slots, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        slots[i].unk00 = 0;
        slots[i].unk04 = 0x190;
        slots[i].unk06 = 0x190;
        slots[i].unk08 = 0;
        slots[i].unk0A = 0;
        slots[i].unk0C = 0;
        slots[i].unk0E = 0;
        slots[i].unk0F = 0;
        slots[i].unk10 = 0;
        slots[i].unk12 = 0;
        slots[i].unk14 = 0xFF;
        slots[i].unk15 = 0xFF;
        slots[i].unk16 = 0xFF;
        slots[i].unk18 = 0;
        slots[i].unk1A = 0;
        slots[i].unk1C = 0x1000;
        slots[i].unk1E = 0x1000;
        slots[i].unk20 = 0;
        slots[i].unk24 = 0;
        slots[i].unk28 = 0;
    }
}
