#include "../../include/types.h"

extern s32 g_main_8011EE8C_LastInitMode;
extern s32 g_main_8010C5E4_StateStack[];

/* --- libc-style externs (not yet decompiled) --- */
extern void func_8002B9A8(s32 state, s32 push);

/* Pops g_main_8010C5E4_StateStack (pre-decrementing the depth counter,
   then reading the entry at the new depth), then notifies via
   func_8002B9A8(state, 0). Sibling of f_main_8002BC44_PushState. */
void f_main_8002BC80_PopState(void) {
    s32 state;

    g_main_8011EE8C_LastInitMode--;
    state = g_main_8010C5E4_StateStack[g_main_8011EE8C_LastInitMode];

    func_8002B9A8(state, 0);
}
