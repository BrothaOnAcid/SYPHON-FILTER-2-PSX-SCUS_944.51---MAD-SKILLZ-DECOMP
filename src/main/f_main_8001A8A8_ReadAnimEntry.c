#include "../../include/types.h"

/* HAN animation reader - see f_main_8001C4F4_ReadKeyframes. Advances the
   animation of a single AnimEntry; dispatched per entry by
   f_main_8001A7BC_UpdateAnimEntries. `unused` is always 1 and never read.
   The owner doubles as the HanCtx (its +0x18/+0x24 hold the node pointers
   and per-channel angle accumulators). */
extern u8 *func_8001C4F4(u8 *src, u32 packed, HanCtx *ctx, u32 mask);
extern void func_80015FF0(void *owner, s16 param);

void f_main_8001A8A8_ReadAnimEntry(AnimOwner *owner, AnimEntry *entry, u32 unused) {
    u8 *p;
    u32 id;
    void (*cb)(void *, u32, u32);
    void *sub;
    u32 chCount;

    p = entry->anim;
    if ((p[0] & 0xF0) != 0xF0)
        return;

    id = p[1];
    entry->unk14 = id;
    if (id == 0xFC && (entry->flags08 & 0x10000000)) {
        p = entry->sub;
        entry->anim = p;
        id = p[1];
        entry->unk14 = id;
    }

    if ((entry->flags20 & 0x4000000) && (entry->flags20 & 0xFF) == id) {
        cb = entry->callback;
        if (cb != 0) {
            sub = entry->sub;
            cb(owner, id | 0x4000000, entry->unk24);
            if (entry->sub != sub)
                return;
        }
    }

    chCount = *(u32 *) ((u8 *) owner->def + 0x24);
    p += 2;
    entry->anim = func_8001C4F4(p + 2, (id << 16) | chCount, (HanCtx *) owner,
                                ((u32) p[0] << 8) | p[1]);

    if (((u8 *) entry->anim)[1] == 0xFC && (entry->flags08 & 0x10000000)) {
        entry->count--;
        if (entry->count == 0) {
            func_80015FF0(owner, entry->unk04);
            return;
        }
        if ((entry->flags20 & 0x10000000) && entry->callback != 0)
            entry->callback(owner, 0x10000000, entry->unk24);
        return;
    }
    func_80015FF0(owner, entry->unk04);
}
