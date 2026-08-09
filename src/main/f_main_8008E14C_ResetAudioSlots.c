#include "../../include/types.h"

extern AudioSlotEntry D_80134D1C[3];

void f_main_8008E14C_ResetAudioSlots(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        D_80134D1C[i].unk0 = -1;
        D_80134D1C[i].unk8 = -1;
        D_80134D1C[i].unk4 = -1;
        D_80134D1C[i].unk6 = -1;
        D_80134D1C[i].unkC = 0xFF;
        D_80134D1C[i].unkD = 0x50;
    }
}
