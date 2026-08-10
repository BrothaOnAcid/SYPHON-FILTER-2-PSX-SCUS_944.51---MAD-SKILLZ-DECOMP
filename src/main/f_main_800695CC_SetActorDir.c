#include "../../include/types.h"

/* guess: sets an actor's pending direction (pendX/Y/Z/W, ActorAction
   +0x8..0x14) from `dir` (or zeroes pendX/Y/Z when dir is NULL, leaving
   pendW untouched). If the resulting pendX/Y/Z is non-zero, it's copied
   into dirX/Y/Z/W (+0x28..0x34) and `state` is used as-is; if it's all
   zero, no copy happens and `state` forces to 6. Either way, unk38 is set
   to that state and the old unk38 value is saved to unk3C. No-op if node,
   its core, or its active action is NULL. */
void f_main_800695CC_SetActorDir(ActorNode *node, Vec4 *dir, s32 state) {
    ActorCore *core;
    ActorAction *act;

    if (node == 0)
        return;

    core = node->core;
    if (core == 0)
        return;

    act = core->act;
    if (act == 0)
        return;

    if (dir == 0) {
        act->pendX = 0;
        act->pendY = 0;
        act->pendZ = 0;
    } else {
        act->pendX = dir->x;
        act->pendY = dir->y;
        act->pendZ = dir->z;
        act->pendW = dir->w;
    }

    if (act->pendX != 0 || act->pendY != 0 || act->pendZ != 0) {
        act->dirX = act->pendX;
        act->dirY = act->pendY;
        act->dirZ = act->pendZ;
        act->dirW = act->pendW;
    } else {
        state = 6;
    }

    {
        s32 prev = act->unk38;
        act->unk38 = state;
        act->unk3C = prev;
    }
}
