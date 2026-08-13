#include "../../include/types.h"

/* Not in asm/800.s: 0x800FC9C4 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c.

   Called from f_main_800FC814_DestroyMmid as func_800FC9C4() with its own
   `obj` argument passed straight through (no new value loaded at the call
   site, same trick as f_main_801050B4_CancelScheduledSlot). Confirms the
   g_main_8012F408_SongTreeListHead list nodes are SongTree (walks +0x18 count / +0x20
   slots exactly like f_main_800F9F68_PlaySongNode does). */

extern SongTree *g_main_8012F408_SongTreeListHead;
extern void f_main_800FA404_DispatchSongSlot(SongTree *tree, s32 idx);

/* guess: for every SongTree in the global list, for every slot whose raw
   +0x1C field equals `obj`, dispatches it (as if to stop/detach it) and
   clears that field - i.e. detaches any song-tree slots still referencing
   `obj` before it's destroyed. */
void f_main_800FC9C4_NotifySlotsByTag(void *obj) {
    SongTree *tree = g_main_8012F408_SongTreeListHead;

    while (tree != 0) {
        s32 i;

        for (i = 0; i < tree->count; i++) {
            u8 *slot = (u8 *) &tree->slots[i];

            if (*(void **) (slot + 0x1C) == obj) {
                f_main_800FA404_DispatchSongSlot(tree, i);
                *(void **) (slot + 0x1C) = 0;
            }
        }

        tree = tree->next;
    }
}
