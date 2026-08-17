#include "../../include/types.h"

/* NOTE: declared elsewhere (f_main_8002A518_InitGame) as a simple
   "last init mode" scalar, but this function increments it and uses it as
   an index into g_main_8010C5E4_StateStack - behaves like a stack-depth
   counter, not a plain mode value. Not reconciled between the two
   interpretations; kept as the same global either way. */
extern s32 g_main_8011EE8C_LastInitMode;
extern s32 g_main_8010C5E4_StateStack[];

/* --- libc-style externs (not yet decompiled) --- */
extern void func_8002B9A8(s32 state, s32 push);

/* Pushes `state` onto g_main_8010C5E4_StateStack (pre-incrementing the
   depth counter first, so index 0 is never written), then notifies via
   func_8002B9A8(state, 1). Sibling of f_main_8002BC80_PopState, which
   decrements (pops). */
void f_main_8002BC44_PushState(s32 state) {
    g_main_8011EE8C_LastInitMode++;
    g_main_8010C5E4_StateStack[g_main_8011EE8C_LastInitMode] = state;

    func_8002B9A8(state, 1);
}
