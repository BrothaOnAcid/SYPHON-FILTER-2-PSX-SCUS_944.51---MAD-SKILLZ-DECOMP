#include "../../include/types.h"

/* guess: called from f_main_80068C38_UpdateActors. Integrates one or both
   of an actor core's position/velocity pairs (posX/Y/Z += velX/Y/Z,
   pos2X/Y/Z += vel2X/Y/Z), gated independently by the low byte of `move1`
   and `move2`. */
void f_main_8006852C_IntegratePositions(ActorNode *node, s32 move1, s32 move2) {
    ActorCore *core = node->core;

    if (move1 & 0xFF) {
        core->posX += core->velX;
        core->posY += core->velY;
        core->posZ += core->velZ;
    }

    if (move2 & 0xFF) {
        core->pos2X += core->vel2X;
        core->pos2Y += core->vel2Y;
        core->pos2Z += core->vel2Z;
    }
}
