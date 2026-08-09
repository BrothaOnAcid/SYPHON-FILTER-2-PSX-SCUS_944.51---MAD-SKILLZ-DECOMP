#include "../../include/types.h"

/* Regular asm/800.s code (not the misclassified block), decompiled via
   m2c. Called once per frame from the player object's state machine
   (asm/800.s, jal at 0x8003A688) - the "song/sound process" tick.
   `self` is just forwarded straight through to
   f_main_8008E4BC_UpdateDangerLevel (register a0 is never touched
   before that call). */

extern s32 g_main_8011F1B0_ActiveSoundCount;
extern s32 g_main_8011F1BC_unk;
extern s32 g_main_8011F1C0_unk;
extern s32 g_main_8011F668_unk;
extern SoundQueueEntry g_main_80134D4C_SoundQueue[8];

extern void f_main_8008E4BC_UpdateDangerLevel(void *self);
extern void f_main_8008D678_UpdateQueuedSound(s32 category, s32 id, void *ctx, void *posArg,
                                               s16 ov1Override, s16 ov2Override);
extern void f_main_8008E878_ScheduleNextAmbient(void);

/* Per-frame sound-system tick: runs f_main_8008E4BC_UpdateDangerLevel
   first (feeds the music "danger" system its new target for this
   frame); if there are any active sounds
   (g_main_8011F1B0_ActiveSoundCount), updates every live SoundQueue entry
   (id != -1) via f_main_8008D678_UpdateQueuedSound, passing &pos only when
   `active` is set;
   finally, if a countdown (g_main_8011F1C0_unk) is running and hasn't yet
   exceeded the elapsed-time budget (g_main_8011F668_unk -
   g_main_8011F1BC_unk), fires f_main_8008E878_ScheduleNextAmbient (which
   eventually triggers the next song via f_main_8008D21C_PlaySound). */
void f_main_8008E990_ProcessSoundQueue(void *self) {
    s32 i;

    f_main_8008E4BC_UpdateDangerLevel(self);

    if (g_main_8011F1B0_ActiveSoundCount != 0) {
        for (i = 0; i < 8; i++) {
            SoundQueueEntry *e = &g_main_80134D4C_SoundQueue[i];

            if (e->id != -1) {
                s32 *pos = e->active != 0 ? e->pos : 0;

                f_main_8008D678_UpdateQueuedSound(e->id, e->subId, e->ctx, pos, -1, -1);
            }
        }
    }

    if (g_main_8011F1C0_unk != -1 &&
        (u32) g_main_8011F1C0_unk < (u32) (g_main_8011F668_unk - g_main_8011F1BC_unk)) {
        f_main_8008E878_ScheduleNextAmbient();
    }
}
