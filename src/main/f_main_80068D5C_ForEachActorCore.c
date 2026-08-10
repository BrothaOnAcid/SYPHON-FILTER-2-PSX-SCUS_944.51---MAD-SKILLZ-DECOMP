#include "../../include/types.h"

extern ActorNode *g_main_8011F160_ActorList;

extern void func_80068618(ActorNode *node, ActorCore *core); /* not yet decompiled - recursive, large */

/* guess: walks the registered-actor list calling func_80068618(node,
   node->core) on each. */
void f_main_80068D5C_ForEachActorCore(void) {
    ActorNode *node = g_main_8011F160_ActorList;

    while (node != 0) {
        ActorCore *core = node->core;
        func_80068618(node, core);
        node = core->next;
    }
}
