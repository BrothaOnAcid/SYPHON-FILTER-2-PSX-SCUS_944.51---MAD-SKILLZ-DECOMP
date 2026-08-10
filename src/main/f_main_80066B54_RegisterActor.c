#include "../../include/types.h"

extern ActorNode *g_main_8011F160_ActorList;

/* guess: registers `node` in the actor list (see g_main_8011F160_ActorList,
   removed by func_80066BE0) if it isn't already present, prepending it to
   the list, then sets core->flags104 bit 0x1000. No-op if node or its core
   is NULL, or if node is already in the list. */
void f_main_80066B54_RegisterActor(ActorNode *node) {
    ActorCore *core;

    if (node == 0)
        return;

    core = node->core;
    if (core == 0)
        return;

    if (g_main_8011F160_ActorList == 0) {
        g_main_8011F160_ActorList = node;
    } else {
        ActorNode *cur = g_main_8011F160_ActorList;

        while (cur != 0 && cur != node)
            cur = cur->core->next;

        if (cur == node)
            return;

        core->next = g_main_8011F160_ActorList;
        g_main_8011F160_ActorList = node;
    }

    core->flags104 |= 0x1000;
}
