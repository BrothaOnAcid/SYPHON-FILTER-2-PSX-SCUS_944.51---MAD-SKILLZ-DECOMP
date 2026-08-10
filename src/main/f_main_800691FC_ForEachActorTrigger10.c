#include "../../include/types.h"

extern ActorNode *g_main_8011F160_ActorList;

extern void func_8006A7E4(ActorNode *node); /* not yet decompiled */

/* guess: walks the registered-actor list; for each actor with unk10 set,
   calls func_8006A7E4(node). */
void f_main_800691FC_ForEachActorTrigger10(void) {
    ActorNode *node = g_main_8011F160_ActorList;

    while (node != 0) {
        ActorNode *next = node->core->next;

        if (node->unk10 != 0)
            func_8006A7E4(node);

        node = next;
    }
}
