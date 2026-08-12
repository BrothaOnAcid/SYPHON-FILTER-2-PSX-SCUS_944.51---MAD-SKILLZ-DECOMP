#include "../../include/types.h"

/* --- not yet decompiled --- */
extern s16 func_8009860C(void);

/* Guessed: readiness counter/level (compared against a threshold of 5). */
extern u32 g_main_8011F604_unk;

/* Returns 0 until g_main_8011F604_unk reaches 5, then defers to
   func_8009860C. */
s16 f_main_80084828_GetValueIfReady(void) {
    if (g_main_8011F604_unk >= 5) {
        return func_8009860C();
    }
    return 0;
}
