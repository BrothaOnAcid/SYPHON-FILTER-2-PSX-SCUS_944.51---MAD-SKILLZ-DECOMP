#include "../../include/types.h"

extern void func_80024684(s32 p1, s32 p2, void *outBuf, s32 p3); /* fills a stack buffer, not yet decompiled */
extern void *func_800F0F4C(void *buf, s32 p0, s32 p3); /* suspicious: blocked 0x800Fxxxx PSYQ library region */

/* Fills a temp buffer via func_80024684(p1, p2, &tmp, p3), gets a result
   pointer from func_800F0F4C(&tmp, p0, p3), then accumulates a VECTOR found
   at tmp+0x14 into *p3 (treated as a VECTOR - offsets 0/4/8), and returns
   the func_800F0F4C result pointer. Same tmp-buffer shape (offset +0x14
   onward) as f_main_800256B0_BuildAndProcess's buffer. */
void *f_main_80025620_AccumulateVector(s32 p0, s32 p1, s32 p2, VECTOR *p3) {
    u8 tmp[0x20];
    VECTOR *delta = (VECTOR *) (tmp + 0x14);
    void *result;

    func_80024684(p1, p2, tmp, (s32) PTR_U32(p3));
    result = func_800F0F4C(tmp, p0, (s32) PTR_U32(p3));

    p3->vx += delta->vx;
    p3->vy += delta->vy;
    p3->vz += delta->vz;

    return result;
}
