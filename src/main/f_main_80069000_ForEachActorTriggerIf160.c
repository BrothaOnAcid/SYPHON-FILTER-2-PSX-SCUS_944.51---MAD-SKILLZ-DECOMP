#include "../../include/types.h"

extern ActorNode *g_main_8011F160_ActorList;

extern void func_8006870C(ActorNode *node, ActorCore *core); /* not yet decompiled, see f_main_80068EB8 */

/* guess: walks the registered-actor list; calls func_8006870C(node, core)
   only when core->unk160 is set (complement of
   f_main_80068EB8_ForEachActorMaybeTrigger's first condition). */
void f_main_80069000_ForEachActorTriggerIf160(void) {
    ActorNode *node = g_main_8011F160_ActorList;

    while (node != 0) {
        ActorCore *core = node->core;

        if (core->unk160 != 0)
            func_8006870C(node, core);

        node = core->next;
    }
}
