#include "../../include/types.h"

/* Not in asm/800.s: 0x800FC6A8 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c.

   The list node type turned out to be SongTree (confirmed by
   f_main_800FC9C4_NotifySlotsByTag walking the same list and indexing
   +0x18/+0x20 as count/slots) - the nodes f_main_800FC728_InitMmidNode
   resolves leaf "MID " tracks against. */
extern SongTree *g_main_8012F408_SongTreeListHead;

/* searches the list for a node whose `tag` field equals `id`; returns the
   match, or 0 if not found (including an empty list). */
SongTree *f_main_800FC6A8_FindById(s32 id) {
    SongTree *node = g_main_8012F408_SongTreeListHead;

    while (node != 0) {
        if (node->tag == id) {
            return node;
        }
        node = node->next;
    }

    return 0;
}
