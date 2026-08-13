#include "../../include/types.h"

extern ActorNode *g_main_8011F160_ActorList;

extern s32 f_main_80011084_RemoveTailNode(void *a);

/* guess: walks the registered-actor list; for each actor whose current-angle
   block has unk20 set, calls f_main_80011084_RemoveTailNode(ang). */
void f_main_80068DA4_ForEachAngTrigger(void) {
    ActorNode *node = g_main_8011F160_ActorList;

    while (node != 0) {
        AngBlock *ang = node->anim->unk0C;

        if (ang->unk20 != 0)
            f_main_80011084_RemoveTailNode(ang);

        node = node->core->next;
    }
}
