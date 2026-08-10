#include "../../include/types.h"

extern ActorNode *g_main_8011F160_ActorList;

extern void func_80011084(AngBlock *ang); /* not yet decompiled - widely shared utility (19 call sites) */

/* guess: walks the registered-actor list; for each actor whose current-angle
   block has unk20 set, calls func_80011084(ang). */
void f_main_80068DA4_ForEachAngTrigger(void) {
    ActorNode *node = g_main_8011F160_ActorList;

    while (node != 0) {
        AngBlock *ang = node->anim->unk0C;

        if (ang->unk20 != 0)
            func_80011084(ang);

        node = node->core->next;
    }
}
