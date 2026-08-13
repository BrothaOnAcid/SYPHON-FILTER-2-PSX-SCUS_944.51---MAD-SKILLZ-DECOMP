#include "../../include/types.h"

/* Blocked-region code (0x800EFD3C-0x80168298, see DOC.md), hand-decoded
   from raw words with toolz/decode_mips.py; uses the alternate gp base
   0x8012EC64. Part of the "resident" streamed-VH memory allocator - see
   ResidentFreeNode's doc comment in types.h. Called by
   f_main_800FC13C_BeginVhLoad's "alt-init" path (rec->flags08 bit2 set)
   to get a fresh buffer of `size` bytes for a SongTree's resident data.

   CAVEAT: the drill-down loop re-searching via `cur->bucketNext` after
   the first hit isn't fully understood (see f_main_80105844_FindFreeNode's
   note) - transcribed for control-flow accuracy from the raw words. */

extern ResidentFreeNode *g_main_8012F484_ResidentFreeHead;
extern ResidentFreeNode *f_main_80105844_FindFreeNode(ResidentFreeNode *n, s32 want);
extern void f_main_80105700_UnlinkFreeNode(ResidentFreeNode *n);
extern void f_main_801057B8_InsertFreeNode(ResidentFreeNode *n);

s32 f_main_80105384_AllocResident(s32 size) {
    ResidentFreeNode *cur;
    ResidentFreeNode *hit;
    s32 base;
    s32 rem;

    size = (size & 0x3f) ? (size + 0x40 - (size & 0x3f)) : size;

    cur = f_main_80105844_FindFreeNode(g_main_8012F484_ResidentFreeHead, size);
    if (cur == 0)
        return 0;

    for (;;) {
        hit = f_main_80105844_FindFreeNode(cur->bucketNext, size);
        if (hit == 0)
            break;
        cur = hit;
    }

    base = cur->addr;
    f_main_80105700_UnlinkFreeNode(cur);

    rem = cur->size - size;
    if (rem != 0) {
        cur->size = rem;
        cur->addr = base + size;
        f_main_801057B8_InsertFreeNode(cur);
    }

    return base;
}
