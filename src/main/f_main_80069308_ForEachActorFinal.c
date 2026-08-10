#include "../../include/types.h"

extern ActorNode *g_main_8011F160_ActorList;

extern void func_800BFECC(ActorNode *node); /* guess: per-actor per-frame subsystem update, sibling of func_800BFE18 */

/* guess: walks the registered-actor list calling func_800BFECC on each
   node. */
void f_main_80069308_ForEachActorFinal(void) {
    ActorNode *node = g_main_8011F160_ActorList;

    while (node != 0) {
        func_800BFECC(node);
        node = node->core->next;
    }
}
