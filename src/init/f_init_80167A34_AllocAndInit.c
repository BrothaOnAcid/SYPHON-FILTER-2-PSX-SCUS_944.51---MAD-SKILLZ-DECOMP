#include "../../include/types.h"

extern void *f_main_80025AD0_AllocDown(s32 size);
extern void func_80167A6C(void *buf);         /* not yet decompiled, 0x4F8 bytes */

/* Allocates a fixed 0x1CF8-byte buffer and runs func_80167A6C to
   initialize it, returning the buffer to the caller. */
void *f_init_80167A34_AllocAndInit(void) {
    void *buf;

    buf = f_main_80025AD0_AllocDown(0x1CF8);
    func_80167A6C(buf);
    return buf;
}
