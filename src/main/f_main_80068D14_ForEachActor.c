#include "../../include/types.h"

extern ActorNode *g_main_8011F160_ActorList;

extern void func_800BFE18(ActorNode *node); /* guess: per-actor per-frame subsystem update */

/* guess: walks the registered-actor list (same list as
   f_main_80068C38_UpdateActors) calling func_800BFE18 on each node. */
void f_main_80068D14_ForEachActor(void) {
    ActorNode *node = g_main_8011F160_ActorList;

    while (node != 0) {
        func_800BFE18(node);
        node = node->core->next;
    }
}
