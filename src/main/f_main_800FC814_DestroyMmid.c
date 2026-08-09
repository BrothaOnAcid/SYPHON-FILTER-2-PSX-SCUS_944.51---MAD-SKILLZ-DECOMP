#include "../../include/types.h"

/* PSYQ audio-sequencer globals; this function is in the blocked region
   (0x800EFD3C-0x80168298) that uses its own gp base 0x8012EC64, not main's
   gp 0x8011EC64. */
extern s32 g_main_8012F410_MmidLock;      /* guess: reentrancy guard */
extern MmidObj *g_main_8012F40C_ActiveMmid;  /* mirrored main-side at
    g_main_8011F40C_ActiveMmid, per user-provided knowledge; a circular
    linked list of active MMID objects, linked via MmidObj.next (+0xC) */

extern void f_main_800FC9C4_NotifySlotsByTag(void *obj);

/* Recursively destroys an MMID container: if not already recursing, first
   detaches any song-tree slots still referencing `obj`
   (f_main_800FC9C4_NotifySlotsByTag); then, if `obj` still has the "MMID"
   magic, recursively destroys each of its trackCount child tracks under a
   reentrancy lock (g_main_8012F410_MmidLock). Finally, unconditionally
   unlinks `obj` from the global active-MMID circular list if present
   there. */
void f_main_800FC814_DestroyMmid(MmidObj *obj) {
    s32 i;
    MmidObj *cur;

    if (g_main_8012F410_MmidLock == 0)
        f_main_800FC9C4_NotifySlotsByTag(obj);

    if (obj->magic == 0x44494D4D) {
        g_main_8012F410_MmidLock = 1;

        for (i = 0; i < obj->trackCount; i++)
            f_main_800FC814_DestroyMmid(obj->tracks[i]);

        g_main_8012F410_MmidLock = 0;
    }

    if (g_main_8012F40C_ActiveMmid == 0)
        return;

    if (g_main_8012F40C_ActiveMmid == obj) {
        g_main_8012F40C_ActiveMmid = obj->next;
        return;
    }

    cur = g_main_8012F40C_ActiveMmid;
    while (cur->next != obj)
        cur = cur->next;
    cur->next = obj->next;
}
