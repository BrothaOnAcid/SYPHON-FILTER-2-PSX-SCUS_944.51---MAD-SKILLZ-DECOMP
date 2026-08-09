#include "../../include/types.h"

extern s32 func_80024490(s32 p1, s32 p2, void *outBuf, s32 p3); /* fills a stack buffer, return value unused here */
extern s32 func_800F0F4C(void *buf, s32 p0, s32 p3); /* suspicious: blocked 0x800Fxxxx PSYQ library region */

/* Fills a temp buffer via func_80024490(p1, p2, &buf, p3), then hands it
   off (with p0/p3) to func_800F0F4C, returning its result. Purpose of the
   buffer/params unconfirmed - both callees still unexamined. */
s32 f_main_800256B0_BuildAndProcess(s32 p0, s32 p1, s32 p2, s32 p3) {
    s32 buf[8]; /* 0x20 bytes, from stack frame (sp+0x10..sp+0x30), unconfirmed layout */

    func_80024490(p1, p2, buf, p3);

    return func_800F0F4C(buf, p0, p3);
}
