#include "../../include/types.h"

extern ActorNode *g_main_8011F160_ActorList;

extern void func_800BD2C0(ActorNode *node); /* not yet decompiled */

/* guess: walks the registered-actor list; for each actor whose core has
   unk160 set, calls func_800BD2C0(node). */
void f_main_80068E64_ForEachActorTrigger160(void) {
    ActorNode *node = g_main_8011F160_ActorList;

    while (node != 0) {
        ActorCore *core = node->core;

        if (core->unk160 != 0)
            func_800BD2C0(node);

        node = core->next;
    }
}
