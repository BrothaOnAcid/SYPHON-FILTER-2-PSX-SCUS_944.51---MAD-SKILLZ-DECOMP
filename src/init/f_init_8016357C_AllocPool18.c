#include "../../include/types.h"

extern void *f_main_80025AD0_AllocDown(s32 size);
extern void func_801633DC(void *ctx, s32 a1, s32 a2); /* not yet decompiled */

/* Allocates a 0x170-byte context at *out and, unless it fails, an 0x48-byte
   (18-entry, stride 4) pointer table at ctx->unk168. Fills that table with
   18 entries: if `stride0` is nonzero, each entry is `base + i*0xA8` (i.e.
   the caller already owns a flat 0x12*0xA8 array at `base` and this just
   indexes into it); otherwise each entry is a freshly-allocated 0xA8-byte
   block. Finishes with func_801633DC(ctx, a1, a2). Returns 0 (leaving *out
   untouched or partially set) if `out` is NULL or the first allocation
   fails; 1 on success. */
s32 f_init_8016357C_AllocPool18(void **out, s32 a1, s32 a2, s32 base) {
    void *ctx;
    void **table;
    s32 i;

    if (out == 0) {
        return 0;
    }
    ctx = f_main_80025AD0_AllocDown(0x170);
    *out = ctx;
    if (ctx == 0) {
        return 0;
    }

    table = (void **) f_main_80025AD0_AllocDown(0x48);
    *(void ***) ((u8 *) ctx + 0x168) = table;

    for (i = 0; i < 0x12; i++) {
        table = *(void ***) ((u8 *) ctx + 0x168);
        if (base != 0) {
            table[i] = (void *) (base + i * 0xA8);
        } else {
            table[i] = f_main_80025AD0_AllocDown(0xA8);
        }
    }

    func_801633DC(ctx, a1, a2);
    return 1;
}
