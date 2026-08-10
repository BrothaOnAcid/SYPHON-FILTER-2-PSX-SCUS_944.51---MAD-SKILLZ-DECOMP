#include "../../include/types.h"

/* guess: fills `out` with a direction/position vector for `node`. If the
   actor core is set, copies it directly (rawX/Y/Z/W). Otherwise falls
   back to the current-angle block's position fields (anim->unk0C,
   AngBlock +0x14/0x18/0x1C), negating Y. Returns 0 if `node` is NULL,
   else 1. */
s32 f_main_80065F90_GetActorDir(ActorNode *node, Vec4 *out) {
    ActorCore *core;
    AngBlock *ang;

    if (node == 0)
        return 0;

    core = node->core;
    if (core != 0) {
        out->x = core->rawX;
        out->y = core->rawY;
        out->z = core->rawZ;
        out->w = core->rawW;
    } else {
        ang = node->anim->unk0C;
        out->x = ang->unk14;
        out->y = -ang->unk18;
        out->z = ang->unk1C;
    }

    return 1;
}
