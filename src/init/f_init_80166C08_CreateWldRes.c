#include "../../include/types.h"

extern void *f_main_80025AD0_AllocDown(s32 size);
extern WldTypeDef *f_init_80166568_InternTypeDef(s32 key, WldRes *res, s32 flags);
extern s32 f_main_80017EAC_InitEagerModel(WldTypeDef *typeDef, s16 *key);

/* Allocates and fills a WldRes for the model identified by `key` (also used
   as f_init_80166568_InternTypeDef's interning key - it's stored verbatim
   into the shared typeDef's dataPtr, see WldTypeDef in types.h). Bails out
   with 1 (and no allocation) if `key` is 0. Unless `flags` bit 0x01000000 is
   set, f_main_80017EAC_InitEagerModel(typeDef, key) also runs - see there
   for the .EMD header fields it pulls out of the raw model data. Returns 0
   on success. */
s32 f_init_80166C08_CreateWldRes(s32 key, s32 tag, s32 flags, WldRes **out) {
    WldRes *res;
    WldTypeDef *typeDef;

    if (key == 0) {
        return 1;
    }

    res = (WldRes *) f_main_80025AD0_AllocDown(0x1C);
    *out = res;

    typeDef = f_init_80166568_InternTypeDef(key, res, flags);
    res->typeDef = typeDef;

    if (!(flags & 0x01000000)) {
        f_main_80017EAC_InitEagerModel(typeDef, (s16 *) (u64) (u32) key);
    }
    typeDef->dataPtr = key;

    res->flags8 = 0;
    res->unk09 = 0x40;
    res->unk0A = 0;
    res->flagsB = 0;
    res->tag = tag;
    res->unk00 = 0;
    res->unk04 = 0;
    res->unk14 = 0;
    res->unk18 = 0;
    return 0;
}
