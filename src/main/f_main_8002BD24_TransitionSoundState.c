#include "../../include/types.h"

extern u8 g_main_8011EE94_SoundState;  /* guess: sound-system mode/state enum */

/* --- libc-style externs (not yet decompiled) --- */
extern void func_80022428(s32 a0, s32 a1, s32 a2, s32 a3);
extern void f_main_8008D59C_ReinitSound(void);
extern void f_main_8008CCF4_ResetSound(void);
extern void func_8002A300(void);
extern void f_main_8002BC44_PushState(s32 flag);
extern void func_800A59AC(s32 a0);

/* Dispatches on g_main_8011EE94_SoundState after two fixed
   func_80022428 setup calls: state 3 -> full re-init (f_main_8008D59C_ReinitSound);
   state 4 -> no reset at all; anything else ->
   f_main_8008CCF4_ResetSound(). State 5 additionally runs
   func_8002A300() afterward. Always finishes with f_main_8002BC44_PushState(2) and
   func_800A59AC(0). */
void f_main_8002BD24_TransitionSoundState(void) {
    u8 state = g_main_8011EE94_SoundState;

    func_80022428(2, 0, 0, 0);
    func_80022428(2, 4, 0, 0);

    if (state == 3) {
        f_main_8008D59C_ReinitSound();
    } else if (state != 4) {
        f_main_8008CCF4_ResetSound();
    }

    if (state == 5)
        func_8002A300();

    f_main_8002BC44_PushState(2);
    func_800A59AC(0);
}
