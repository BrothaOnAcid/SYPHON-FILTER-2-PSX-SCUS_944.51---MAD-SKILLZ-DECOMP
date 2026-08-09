#include "../../include/types.h"

/* --- libc-style externs (not yet decompiled) --- */
extern s32 func_80025AC0(void *ptr);

/* Zeroes a run of words working backward from `ptr - 4`. The word count is
   derived from func_80025AC0(ptr) minus two link-time-constant terms
   (0x11B90/4 and 0x1D4C0 words) - almost certainly a "clear the unused
   tail of an overlay/arena" calculation tied to this binary's exact link
   layout; the constants aren't meaningful independent of that. */
void f_init_8015CF84_ClearOverlayTail(u32 *ptr) {
    u32 *cur = ptr - 1;
    s32 count = (func_80025AC0(ptr) >> 2) - 2;
    s32 i;

    count -= (0x11B90 >> 2);
    count += (s32) 0xFFFE2B40; /* == -0x1D4C0 */

    for (i = 0; i < count; i++) {
        *cur = 0;
        cur--;
    }
}
