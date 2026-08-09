#include "../../include/types.h"

extern u8 g_main_8011F1A8_ResetPending;      /* guess: gates this whole reset */
extern SongTree *g_main_80134CF4_SongTrees[4];
extern MmidObj *g_main_8011F8FC_CurrentMmid; /* guess: currently playing MMID */
extern MmidObj *g_main_8011F8F0_QueuedMmid;  /* guess: queued/next MMID */
extern SoundQueueEntry g_main_80134D4C_SoundQueue[8];
extern s32 g_main_8011F1B0_ActiveSoundCount;
extern s32 g_main_8011F918_unk;

/* --- libc-style externs (not yet decompiled) --- */
extern void func_800FC350(SongTree *tree);
extern void f_main_800FC814_DestroyMmid(MmidObj *obj);

/* Sound-system reset, only runs if g_main_8011F1A8_ResetPending is set:
   tears down all 4 category song trees (func_800FC350), destroys the
   current and queued MMID objects if present, resets the 8-slot
   g_main_80134D4C_SoundQueue (id/subId = -1, ctx/active = 0), then clears
   the active-sound counter, g_main_8011F918_unk, and the pending flag
   itself. */
void f_main_8008CCF4_ResetSound(void) {
    s32 i;

    if (g_main_8011F1A8_ResetPending == 0)
        return;

    for (i = 0; i < 4; i++)
        func_800FC350(g_main_80134CF4_SongTrees[i]);

    if (g_main_8011F8FC_CurrentMmid != 0)
        f_main_800FC814_DestroyMmid(g_main_8011F8FC_CurrentMmid);

    if (g_main_8011F8F0_QueuedMmid != 0)
        f_main_800FC814_DestroyMmid(g_main_8011F8F0_QueuedMmid);

    for (i = 0; i < 8; i++) {
        SoundQueueEntry *slot = &g_main_80134D4C_SoundQueue[i];

        slot->id = -1;
        slot->subId = -1;
        slot->ctx = 0;
        slot->active = 0;
    }

    g_main_8011F1B0_ActiveSoundCount = 0;
    g_main_8011F918_unk = 0;
    g_main_8011F1A8_ResetPending = 0;
}
