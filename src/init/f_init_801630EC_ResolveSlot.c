#include "../../include/types.h"

extern void func_8002D3A8(s16 id, s32 *outValue, LookupRecord **outRecord);
extern void func_80066B18(ResourceSlot *slot);

/* Resolves `slot`'s lookup record by id (via func_8002D3A8), short-
   circuiting to "already done" (1) if `slot->sub` is already set. If the
   lookup fails (no record, or a zero value where one's required) it
   still reports success. When `requireCount` is set, a record with
   `count < 2` is treated as "nothing to do yet" (bails out, still
   returns 1). Otherwise, self-links the record's `next` (empty-list
   init), and - if `initExtra` is set - runs func_80066B18(slot) and
   clears/fills a few fields on the resulting slot->sub.

   Called (as thin wrappers) from f_init_801631C0_ResolveSlotAndInit
   (initExtra=1) and f_init_801631E4_ResolveSlot (initExtra=0).

   CAVEAT: struct field layout (ResourceSlot/ResourceSub/LookupRecord in
   types.h) is a best-effort guess from raw offsets; not verified. */
s32 f_init_801630EC_ResolveSlot(ResourceSlot *slot, u8 requireCount, u8 initExtra) {
    s32 value = 0;
    LookupRecord *rec;

    if (slot->sub != 0)
        return 1;

    func_8002D3A8(slot->id, &value, &rec);

    if (rec == 0)
        return 1;
    if (value == 0)
        return 1;

    if (requireCount != 0 && rec->count < 2)
        return 1;

    rec->next = (u8 *) rec + 8;

    if (initExtra == 0)
        return 1;

    func_80066B18(slot);
    slot->sub->unk124 = 0;
    slot->sub->unk128 = 0;
    slot->sub->unk140 = value;

    return 1;
}
