#include "../../include/types.h"

/* Regular asm/800.s code (not the misclassified block), decompiled via
   m2c. Called from f_main_8008E990_ProcessSoundQueue once the countdown
   (g_main_8011F1C0_unk) elapses. */

extern s32 g_main_8011F910_unk;   /* guess: pending ambient sound category */
extern s32 g_main_8011F920_unk;   /* guess: pending ambient sound id */
extern void *g_main_8011F1B8_unk;   /* guess: pending ambient ctx */
extern void *g_main_8011F1B4_unk;   /* guess: pending ambient posArg */
extern s32 g_main_8011F1BC_unk;
extern s32 g_main_8011F1C0_unk;
extern s32 g_main_8011F668_unk;
extern u8 g_main_8011F924_unk;    /* guess: alternates which branch runs each cycle */

extern s32 func_800F41F0(void);   /* PS1 BIOS rand() (A0 table index 0x2F), left extern */
extern void f_main_8008D4B8_StopQueuedSound(s32 category, s32 id);
extern s32 f_main_8008D21C_PlaySound(s32 category, s32 id, void *ctx, void *posArg);

/* guess: picks a random delay (6-17 ticks normally; occasionally
   1-5 after an extra roll in the f_main_8008D4B8_StopQueuedSound branch),
   re-arms the countdown (g_main_8011F1C0_unk/g_main_8011F1BC_unk) for
   f_main_8008E990_ProcessSoundQueue, and alternates (via
   g_main_8011F924_unk) between replaying the pending ambient sound
   through f_main_8008D21C_PlaySound and stopping it instead. */
void f_main_8008E878_ScheduleNextAmbient(void) {
    s32 delay = (func_800F41F0() % 12) + 6;

    if (g_main_8011F924_unk == 1) {
        s32 roll;

        f_main_8008D4B8_StopQueuedSound(g_main_8011F910_unk, g_main_8011F920_unk);
        roll = func_800F41F0();
        if (roll % 10 >= 6) {
            delay = (func_800F41F0() % 5) + 1;
        }
    } else {
        f_main_8008D21C_PlaySound(g_main_8011F910_unk, g_main_8011F920_unk,
                                   g_main_8011F1B8_unk, g_main_8011F1B4_unk);
    }

    g_main_8011F1C0_unk = delay;
    g_main_8011F1BC_unk = g_main_8011F668_unk;
    g_main_8011F924_unk ^= 1;
}
