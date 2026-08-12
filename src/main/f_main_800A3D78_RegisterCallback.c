#include "../../include/types.h"

/* --- not yet decompiled --- */
extern void *func_800A3BE8(void *a0, void *a1, s32 a2, s32 a3, void *cb);
/* Used only by address here - never `jal`'d directly, i.e. this is passed
   around as a callback pointer. */
extern void func_800A3D1C(void);

/* Forwards p0/p1/p2 to func_800A3BE8 with a fixed callback
   (func_800A3D1C) and a zeroed 4th arg, then stashes `value` into the
   returned object's +0x14 field. `unused` (original a3) is spilled to the
   stack but never read back - dead parameter. */
void f_main_800A3D78_RegisterCallback(void *p0, void *p1, s32 p2, s32 unused, s32 value) {
    u8 *obj = (u8 *) func_800A3BE8(p0, p1, p2, 0, (void *) func_800A3D1C);

    (void) unused;
    *(s32 *) (obj + 0x14) = value;
}
