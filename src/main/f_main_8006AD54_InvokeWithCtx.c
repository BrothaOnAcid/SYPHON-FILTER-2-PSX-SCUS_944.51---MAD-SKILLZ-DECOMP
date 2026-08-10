#include "../../include/types.h"

/* guess: builds a scratch CallbackCtx on the stack (copying `src`'s first
   8 words in when non-NULL, and setting flags28 bit 0 to whether `src`
   was provided) and invokes `fn(context, &ctx, src)`. Matches the
   original asm's uninitialized read of flags28 before masking it -
   whatever bits were already on the stack above bit 0 pass through
   unchanged. */
void f_main_8006AD54_InvokeWithCtx(void (*fn)(void *, CallbackCtx *, s32 *), void *context, s32 *src) {
    CallbackCtx ctx;

    ctx.flag04 = 0;

    if (src != 0) {
        s32 i;
        for (i = 0; i < 8; i++)
            ctx.data[i] = src[i];
    }

    ctx.flags28 = (ctx.flags28 & ~1) | (src != 0);

    fn(context, &ctx, src);
}
