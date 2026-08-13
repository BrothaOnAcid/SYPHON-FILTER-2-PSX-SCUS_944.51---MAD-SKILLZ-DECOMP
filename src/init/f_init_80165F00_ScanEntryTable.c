#include "../../include/types.h"

extern s32 D_80168954[]; /* not yet decompiled: 0x43-entry table, stride 4 */

extern s32 func_80165DE8(s32 a0, s32 a1, void *ctx);   /* not yet decompiled */
extern s32 func_8002576C(void *ctx);                    /* not yet decompiled */
void f_init_80165E48_PackFrame(void *src, s32 index);
extern void func_80165D48(void *ctx);                     /* not yet decompiled */

/* Walks the 67-entry D_80168954 table; for each index, resolves
   (arg0, D_80168954[i]) into a local context via func_80165DE8. Only when
   that succeeds (low byte nonzero) does anything else happen this
   iteration: func_8002576C(ctx) runs, and if IT succeeds too, the running
   result is reset to 0 (i.e. a later successful func_8002576C wins over an
   earlier func_80165DE8 hit); then f_init_80165E48_PackFrame(ctx, i) always runs, and
   func_80165D48(ctx) additionally runs on the very first iteration
   (i == 0). Returns the (low byte of the) last func_80165DE8 result that
   wasn't cleared by func_8002576C - carried over from whichever iteration
   last touched it. */
u8 f_init_80165F00_ScanEntryTable(s32 arg0) {
    u8 buf[0x30];
    s32 result = 0;
    s32 i;

    for (i = 0; i < 0x43; i++) {
        result = func_80165DE8(arg0, D_80168954[i], buf);

        if (result & 0xFF) {
            if (func_8002576C(buf) != 0) {
                result = 0;
            }
            f_init_80165E48_PackFrame(buf, i);
            if (i == 0) {
                func_80165D48(buf);
            }
        }
    }

    return (u8) result;
}
