#include "../../include/types.h"

extern void func_80017EAC(void *obj, s32 owner);
extern void *func_80166568(s32 a0, s32 a1, s32 flags);

/* arg0: owner/key value (0 => no-op); arg1: flags, bit 0x01000000 skips the
   func_80017EAC link-back step. */
void *f_init_80166D14_AllocLinked(s32 arg0, s32 arg1) {
    void *obj;

    if (arg0 == 0)
        return 0;

    obj = func_80166568(arg0, 0, arg1);

    if (!(arg1 & 0x01000000))
        func_80017EAC(obj, arg0);

    *(s32 *) ((u8 *) obj + 0x20) = arg0;
    return obj;
}
