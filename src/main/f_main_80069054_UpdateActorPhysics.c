#include "../../include/types.h"

extern ActorNode *g_main_8011F160_ActorList;

extern s32 g_main_8010DD34_unk;
extern s32 g_main_8010DD38_unk;
extern s32 g_main_8010DD3C_unk;

extern void func_800397A0(ActorNode *node); /* not yet decompiled */

/* guess: per-actor physics/trigger pass. Skips an actor entirely if its
   angle block's unk20 target has its own +0x20 field set (some kind of
   "already linked/handled" marker), unless core->unk184 is set. Otherwise:
   if core->unk15C is set, defers to func_800397A0(node); else, while
   flags104 bit 0x200 is set (and, when core->unk160 is set, the anim's
   flags0A bit 2 is also set), accumulates the three global deltas into
   core->unk70/74/78. */
void f_main_80069054_UpdateActorPhysics(void) {
    ActorNode *node;

    for (node = g_main_8011F160_ActorList; node != 0; node = node->core->next) {
        ActorCore *core = node->core;

        if (core->unk184 == 0) {
            AngBlock *ang = node->anim->unk0C;
            void *target = ang->unk20;
            s32 targetFlag = (target != 0) ? *(s32 *) ((u8 *) target + 0x20) : 0;

            if (targetFlag != 0)
                continue;
        }

        if (core->unk15C != 0) {
            func_800397A0(node);
            continue;
        }

        if (!(core->flags104 & 0x200))
            continue;

        if (core->unk160 != 0 && !(node->anim->flags0A & 0x2))
            continue;

        core->unk70 += g_main_8010DD34_unk;
        core->unk74 += g_main_8010DD38_unk;
        core->unk78 += g_main_8010DD3C_unk;
    }
}
