#include "../../include/types.h"

extern WldTypeDef *f_init_80166568_InternTypeDef(s32 key, WldRes *res, s32 flags);
extern s32 f_main_80017EAC_InitEagerModel(WldTypeDef *typeDef, s16 *key);

/* Same typeDef resolve/init sequence as f_init_80166C08_CreateWldRes, minus
   the WldRes wrapper allocation (interns/creates a WldTypeDef directly, no
   caller-visible handle). arg0: owner/key value (0 => no-op); arg1: flags,
   bit 0x01000000 skips the f_main_80017EAC_InitEagerModel link-back step. */
WldTypeDef *f_init_80166D14_AllocLinked(s32 arg0, s32 arg1) {
    WldTypeDef *typeDef;

    if (arg0 == 0)
        return 0;

    typeDef = f_init_80166568_InternTypeDef(arg0, 0, arg1);

    if (!(arg1 & 0x01000000))
        f_main_80017EAC_InitEagerModel(typeDef, (s16 *) (u64) (u32) arg0);

    typeDef->dataPtr = arg0;
    return typeDef;
}
