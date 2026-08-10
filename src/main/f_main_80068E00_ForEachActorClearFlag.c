#include "../../include/types.h"

extern ActorNode *g_main_8011F160_ActorList;

extern void func_80066F38(ActorNode *node); /* not yet decompiled */

/* guess: walks the registered-actor list, calling func_80066F38(node) then
   clearing flags104 bits 0x400/0x1 on each actor's core. */
void f_main_80068E00_ForEachActorClearFlag(void) {
    ActorNode *node = g_main_8011F160_ActorList;

    while (node != 0) {
        ActorCore *core = node->core;
        ActorNode *next = core->next;

        func_80066F38(node);
        core->flags104 &= ~0x401;

        node = next;
    }
}
