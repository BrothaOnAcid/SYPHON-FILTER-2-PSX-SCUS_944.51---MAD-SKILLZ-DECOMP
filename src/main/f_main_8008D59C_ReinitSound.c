#include "../../include/types.h"

extern u8 g_main_8011F1A8_ResetPending;      /* guess: gates this whole reset */
extern u8 g_main_8011F1A7_unk;               /* guess: a second, adjacent flag */
extern SongTree *g_main_80134CF4_SongTrees[6]; /* NOTE: this function's loop
    touches indices 0-5 and explicitly skips index 4 - suggests the real
    array is 6 entries, wider than the 4 that f_main_8008CCF4_ResetSound
    unconditionally processes; not yet reconciled. */
extern SoundQueueEntry g_main_80134D4C_SoundQueue[8];
extern s32 g_main_8011F1B0_ActiveSoundCount;

/* --- libc-style externs (not yet decompiled) --- */
extern void func_800FA7DC(SongTree *tree);
extern void func_8008EA90(void);
extern void func_800F9210(s32 a0);

/* "Full" sound re-init, only runs if g_main_8011F1A8_ResetPending is set
   (same gate as f_main_8008CCF4_ResetSound, but note this function does
   NOT clear that flag itself). Tears down g_main_80134CF4_SongTrees[0..5]
   via func_800FA7DC, skipping index 4 and any NULL entry; resets the
   8-slot g_main_80134D4C_SoundQueue; clears
   g_main_8011F1B0_ActiveSoundCount and g_main_8011F1A7_unk; then calls
   func_8008EA90() and func_800F9210(0). */
void f_main_8008D59C_ReinitSound(void) {
    s32 i;

    if (g_main_8011F1A8_ResetPending == 0)
        return;

    for (i = 0; i < 6; i++) {
        SongTree *tree = g_main_80134CF4_SongTrees[i];

        if (i == 4)
            continue;
        if (tree == 0)
            continue;

        func_800FA7DC(tree);
    }

    for (i = 0; i < 8; i++) {
        SoundQueueEntry *slot = &g_main_80134D4C_SoundQueue[i];

        slot->id = -1;
        slot->subId = -1;
        slot->ctx = 0;
        slot->active = 0;
    }

    g_main_8011F1B0_ActiveSoundCount = 0;
    g_main_8011F1A7_unk = 0;

    func_8008EA90();
    func_800F9210(0);
}
