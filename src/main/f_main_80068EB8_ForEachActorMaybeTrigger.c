#include "../../include/types.h"

extern ActorNode *g_main_8011F160_ActorList;

extern void func_8006870C(ActorNode *node, ActorCore *core); /* not yet decompiled, also used by f_main_80068F28/f_main_80069000 */

/* guess: walks the registered-actor list; skips actors whose core has
   unk160 set or whose state byte (unk102) is 5, otherwise calls
   func_8006870C(node, core). */
void f_main_80068EB8_ForEachActorMaybeTrigger(void) {
    ActorNode *node = g_main_8011F160_ActorList;

    while (node != 0) {
        ActorCore *core = node->core;

        if (core->unk160 == 0 && core->unk102 != 5)
            func_8006870C(node, core);

        node = core->next;
    }
}
