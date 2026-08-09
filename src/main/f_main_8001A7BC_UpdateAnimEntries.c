#include "../../include/types.h"

/* Guessed: per-frame callback invoked at the end of this function with the
   owner in a0 (NULL = skip). */
extern void (*g_main_8011ED90_Callback)(void *owner);

/* Per-entry HAN animation reader - see f_main_8001A8A8_ReadAnimEntry.
   `unused` is always 1 and never read by the callee. */
extern void f_main_8001A8A8_ReadAnimEntry(AnimOwner *owner, AnimEntry *entry, u32 unused);

/* Dispatches the per-frame animation update for `owner`. Pass 1 walks the
   +0x20 entry chain and updates entries whose id (+0x28) is 0 or 0x4000; pass
   2 re-walks it and updates entries with id 0x8000. A no-op unless both +0x18
   and +0x24 are set; ends by calling the g_main_8011ED90 callback (if any). */
void f_main_8001A7BC_UpdateAnimEntries(AnimOwner *owner) {
    ListNode *node;
    AnimEntry *entry;
    void (*cb)(void *);

    if (owner->anim18 == 0 || owner->extra24 == 0)
        return;

    node = owner->entries;
    while (node != 0) {
        entry = (AnimEntry *) node->owner;
        node = node->next;
        if (entry->id == 0 || entry->id == 0x4000)
            f_main_8001A8A8_ReadAnimEntry(owner, entry, 1);
    }

    node = owner->entries;
    while (node != 0) {
        entry = (AnimEntry *) node->owner;
        node = node->next;
        if (entry->id == 0x8000)
            f_main_8001A8A8_ReadAnimEntry(owner, entry, 1);
    }

    cb = g_main_8011ED90_Callback;
    if (cb != 0)
        cb(owner);
}
