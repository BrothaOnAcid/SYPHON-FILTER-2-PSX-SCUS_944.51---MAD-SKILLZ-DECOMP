#include "../../include/types.h"

/* Guessed: fills a 3x3-short-grid-shaped stack buffer (same byte layout
   observed in f_main_80025338_BuildMatrix's output - stride-6 rows), not
   yet decompiled. */
extern s32 func_80024490(s32 p1, s32 p2, void *outBuf, s32 p3);

/* Calls func_80024490(p1, p2, &tmp, p3) then extracts one "column" (offsets
   +4, +0xA, +0x10 within tmp - stride 6, matching f_main_80025338's row
   layout) of 3 shorts, widening each into *out (a VECTOR). */
void f_main_80025238_ExtractVector(s32 p1, s32 p2, VECTOR *out, s32 p3) {
    s16 tmp[9];

    func_80024490(p1, p2, tmp, p3);

    out->vx = tmp[2]; /* +0x14 */
    out->vy = tmp[5]; /* +0x1A */
    out->vz = tmp[8]; /* +0x20 */
}
