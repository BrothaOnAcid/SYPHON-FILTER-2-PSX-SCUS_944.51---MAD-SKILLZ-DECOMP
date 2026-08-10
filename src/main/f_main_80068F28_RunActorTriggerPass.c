#include "../../include/types.h"

extern ActorNode *g_main_8011F160_ActorList;

extern void func_8006870C(ActorNode *node, ActorCore *core); /* not yet decompiled, see f_main_80068EB8 */
extern void func_800B99F4(void);      /* guess: begin-pass hook, not yet decompiled */
extern void func_800B9958(s32 arg0);  /* guess: end-pass hook, not yet decompiled */

/* guess: if any registered actor's state byte (core->unk102) is 5, runs
   the same trigger pass as f_main_80068EB8_ForEachActorMaybeTrigger,
   bracketed by func_800B99F4()/func_800B9958(0). */
void f_main_80068F28_RunActorTriggerPass(void) {
    ActorNode *node;
    s32 found = 0;

    for (node = g_main_8011F160_ActorList; node != 0; node = node->core->next) {
        if (node->core->unk102 == 5)
            found = 1;
        if (found)
            break;
    }

    if (!found)
        return;

    func_800B99F4();

    for (node = g_main_8011F160_ActorList; node != 0; node = node->core->next) {
        ActorCore *core = node->core;

        if (core->unk160 == 0 && core->unk102 != 5)
            func_8006870C(node, core);
    }

    func_800B9958(0);
}
