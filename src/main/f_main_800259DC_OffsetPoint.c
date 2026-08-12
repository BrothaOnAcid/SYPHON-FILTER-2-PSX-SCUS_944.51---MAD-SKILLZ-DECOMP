#include "../../include/types.h"

/* Guessed: BSS-resident (zero at load, presumably filled in elsewhere)
   screen/viewport origin pair. */
extern u16 g_main_8013E6A8_OriginX;
extern u16 g_main_8013E6AA_OriginY;

/* Writes `src`'s +0x4/+0x6 fields, offset by the current origin, into
   `dst`'s +0x0/+0x2 fields (a separate, smaller struct - just an s16 pair).
   Returns 0x16 if `src` is NULL (nothing written), else 0. */
s32 f_main_800259DC_OffsetPoint(u8 *src, s16 *dst) {
    if (src == 0) {
        return 0x16;
    }
    dst[0] = (s16) (*(u16 *) (src + 4) + g_main_8013E6A8_OriginX);
    dst[1] = (s16) (*(u16 *) (src + 6) + g_main_8013E6AA_OriginY);
    return 0;
}
