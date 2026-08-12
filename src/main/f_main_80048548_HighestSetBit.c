#include "../../include/types.h"

/* Scans `obj`'s +0x8 flags field for the highest set bit among bits 1-9
   (bit 0 is never tested) and returns (bitIndex + 1): 9 if bit 9 (0x200) is
   set, else the highest set bit among 8..1 plus one, else 0 if none of
   bits 1-8 are set. Guess: picks the highest-priority slot out of a 9-slot
   ownership/availability bitmask (weapon slot selection?). */
s32 f_main_80048548_HighestSetBit(u8 *obj) {
    s32 flags = *(s32 *) (obj + 8);
    s32 i;

    if (flags & 0x200) {
        return 9;
    }
    for (i = 8; i != 0; i--) {
        if (flags & (1 << i)) {
            return i + 1;
        }
    }
    return 0;
}
