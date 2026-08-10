#include "../../include/types.h"

extern ActorNode *g_main_8011F160_ActorList;

extern s32 func_80066300(ActorNode *node);              /* not yet decompiled */
extern s32 func_800685B0(ActorNode *node, ActorCore *core); /* not yet decompiled */
extern void func_80066BE0(ActorNode *node);              /* removes node from the actor list, see g_main_8011F160_ActorList */

/* guess: per-actor expiry/retry counter. Skips actors with unk103 == 0
   (disabled). Otherwise resets the low byte of flags104 to 0 whenever
   func_80066300 fails or func_800685B0 succeeds; if both instead "hold"
   (func_80066300 succeeds and func_800685B0 fails), increments that
   counter, and once it reaches unk103, removes the actor. */
void f_main_8006924C_UpdateActorExpiry(void) {
    ActorNode *node;

    for (node = g_main_8011F160_ActorList; node != 0;) {
        ActorCore *core = node->core;
        ActorNode *next = core->next;
        u8 *counter = (u8 *) &core->flags104;

        if (core->unk103 != 0) {
            if (func_80066300(node) == 0 || func_800685B0(node, core) != 0) {
                *counter = 0;
            } else {
                (*counter)++;
            }

            if (*counter >= core->unk103)
                func_80066BE0(node);
        }

        node = next;
    }
}
