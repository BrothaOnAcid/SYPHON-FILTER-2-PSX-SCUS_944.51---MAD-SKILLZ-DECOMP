#include "../../include/types.h"

/* Walks the ListHead embedded at `obj`+0x14A0 (list of nodes whose `owner`
   field points at a record starting with an s32 key), looking for a node
   whose record's key equals `key`. Returns the matching record, or NULL. */
void *f_main_80036C40_FindEntryByKey(u8 *obj, s32 key) {
    ListNode *n = ((ListHead *) (obj + 0x14A0))->first;

    while (n != 0) {
        s32 *entry = (s32 *) n->owner;

        if (*entry == key) {
            return entry;
        }
        n = n->next;
    }
    return 0;
}
