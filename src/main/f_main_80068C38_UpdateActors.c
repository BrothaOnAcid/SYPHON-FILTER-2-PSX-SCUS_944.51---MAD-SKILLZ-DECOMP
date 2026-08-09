#include "../../include/types.h"

/* Head of the registered-actor linked list (nodes added by func_80066B54,
   removed by func_80066BE0). */
extern ActorNode *g_main_8011F160_ActorList;

/* Gated per-actor anim update. */
extern void f_main_8001A740_UpdateAnims(AnimOwner *owner);

/* Recomputed recoil rotation/direction from the current angles - see
   f_main_8006C730_UpdateRecoil. */
extern void f_main_8006C730_UpdateRecoil(ActorNode *node);

/* Per-frame update of every registered actor. Drives the anim update, clears
   the core +0x104 bit 0x800, and - while the active action's +0x40 bit 0x10 is
   set - triggers f_main_8006C730_UpdateRecoil once the anim angle at
   extra24+0x72 (abs) grows past 0x400, then clears that bit. */
void f_main_80068C38_UpdateActors(void) {
    ActorNode *node;
    ActorCore *core;
    ActorNode *next;
    ActorAction *act;
    s16 angle;

    node = g_main_8011F160_ActorList;
    while (node != 0) {
        core = node->core;
        next = core->next;
        f_main_8001A740_UpdateAnims(node->anim);
        core->flags104 &= ~0x800;
        act = core->act;
        if (act != 0 && (act->flags40 & 0x10) != 0) {
            angle = *(s16 *) ((u8 *) node->anim->extra24 + 0x72);
            if (angle < 0)
                angle = -angle;
            if (angle >= 0x401) {
                f_main_8006C730_UpdateRecoil(node);
                act->flags40 &= ~0x10;
            }
        }
        node = next;
    }
}
