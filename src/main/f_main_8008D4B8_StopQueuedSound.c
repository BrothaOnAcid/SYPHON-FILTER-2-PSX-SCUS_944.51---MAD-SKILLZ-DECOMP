#include "../../include/types.h"

extern SongTree *g_main_80134CF4_SongTrees[];
extern SoundQueueEntry g_main_80134D4C_SoundQueue[8];
extern s32 g_main_8011F1B0_ActiveSoundCount; /* gp-rel */

extern s32 f_main_800FA404_DispatchSongSlot(SongTree *tree, s32 idx);
extern s32 func_800FA680(SongTree *tree, s32 idx);

/* Cancels a queued/playing sound: dispatches `id` off `category`'s song
   tree, then looks it up in func_800FA680's tracking table; if tracked,
   scans g_main_80134D4C_SoundQueue for the matching (category, id) slot
   and frees it (id/subId/ctx/active reset), decrementing the active
   count. Called from various gameplay sites (e.g. weapon/ammo handling)
   to stop a sound that was previously registered via
   f_main_8008D21C_PlaySound. */
void f_main_8008D4B8_StopQueuedSound(s32 category, s32 id) {
    s32 i;

    f_main_800FA404_DispatchSongSlot(g_main_80134CF4_SongTrees[category], id);
    if (func_800FA680(g_main_80134CF4_SongTrees[category], id) == 0)
        return;

    for (i = 0; i < 8; i++) {
        SoundQueueEntry *slot = &g_main_80134D4C_SoundQueue[i];

        if (slot->id != category || slot->subId != id)
            continue;

        slot->id = -1;
        slot->subId = -1;
        g_main_8011F1B0_ActiveSoundCount--;
        slot->ctx = 0;
        slot->active = 0;
        return;
    }
}
