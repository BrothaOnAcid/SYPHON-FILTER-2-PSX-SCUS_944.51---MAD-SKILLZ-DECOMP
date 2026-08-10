#include "../../include/types.h"

extern ActorNode *g_main_8011F160_ActorList;

/* not yet decompiled; the original asm temporarily swaps $sp to a fixed
   PSX scratchpad address for the func_80066CD4 call only (a codegen/perf
   detail not represented here). */
extern void func_80066CD4(ActorNode *node);
extern void func_800A9AD8(ActorNode *node);

/* guess: walks the registered-actor list; for each actor whose core has
   unk160 set, calls func_80066CD4(node) then func_800A9AD8(node). */
void f_main_80069180_ForEachActorTrigger160Pair(void) {
    ActorNode *node = g_main_8011F160_ActorList;

    while (node != 0) {
        ActorCore *core = node->core;
        ActorNode *next = core->next;

        if (core->unk160 != 0) {
            func_80066CD4(node);
            func_800A9AD8(node);
        }

        node = next;
    }
}
