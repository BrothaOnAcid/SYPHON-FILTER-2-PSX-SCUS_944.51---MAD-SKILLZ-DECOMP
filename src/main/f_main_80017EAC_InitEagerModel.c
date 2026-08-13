#include "../../include/types.h"

extern void func_8001BA68(s32 key, WldTypeDef *typeDef); /* not yet decompiled */

/* Called by f_init_80166C08_CreateWldRes right after resolving `typeDef` via
   f_init_80166568_InternTypeDef, unless CreateWldRes's own `flags` had bit
   0x01000000 set. If typeDef->flags28 has bit 0x4000000 set, copies the
   bounding-box min/max out of the raw model data at `key` (file offsets
   +0xA/+0xC/+0xE/+0x10/+0x12/+0x14) into typeDef's min0/1/2 and max0/1/2 -
   the first bytes of the .EMD format we've pulled out (see WldTypeDef -
   confirmed bbox by f_init_8015E370_GrowBoxToSize /
   f_init_8015E42C_ShrinkBoxToSize). Else if flags28 bit 0x1000000 is set,
   defers to func_8001BA68 instead. */
s32 f_main_80017EAC_InitEagerModel(WldTypeDef *typeDef, s16 *key) {
    u32 flags = typeDef->flags28;

    if (flags & 0x4000000) {
        typeDef->min0 = key[5];  /* +0xA */
        typeDef->min1 = key[6];  /* +0xC */
        typeDef->min2 = key[7];  /* +0xE */
        typeDef->max0 = key[8];  /* +0x10 */
        typeDef->max1 = key[9];  /* +0x12 */
        typeDef->max2 = key[10]; /* +0x14 */
    } else if (flags & 0x1000000) {
        func_8001BA68((s32) (u64) key, typeDef);
    }
    return 0;
}
