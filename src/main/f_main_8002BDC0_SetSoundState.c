#include "../../include/types.h"

extern u8 g_main_8011EE94_SoundState; /* guess: sound-system mode/state enum */

void f_main_8002BDC0_SetSoundState(u8 state) {
    g_main_8011EE94_SoundState = state;
}
