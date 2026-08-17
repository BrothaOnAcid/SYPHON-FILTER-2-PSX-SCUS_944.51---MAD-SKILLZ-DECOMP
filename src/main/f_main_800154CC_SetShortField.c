#include "../../include/types.h"

extern s32 func_8001543C(s32 id, u16 *out); /* not yet decompiled */

s32 f_main_800154CC_SetShortField(void *w, s32 id) {
    u16 val;
    s32 err = func_8001543C(id, &val);

    if (err == 0) {
        *(s16 *) ((u8 *) w + 4) = val;
    }
    return err;
}
