#include "../../include/types.h"

extern void func_80010B14(s32 *src, s32 *dst); /* normalizes a 3-word vector, see f_main_8006C730_UpdateRecoil */

/* guess: copies `dir` into the actor's recoil direction vector
   (ActorAction +0x18..0x24, same fields f_main_8006C730_UpdateRecoil
   writes). If the Y component ends up non-zero, it's cleared and the
   vector is renormalized in place. No-op if node, its core, or its
   active action is NULL. */
void f_main_8006969C_SetActorRecoilDir(ActorNode *node, Vec4 *dir) {
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

    act->unk18 = dir->x;
    act->unk1C = dir->y;
    act->unk20 = dir->z;
    act->unk24 = dir->w;

    if (act->unk1C != 0) {
        act->unk1C = 0;
        func_80010B14(&act->unk18, &act->unk18);
    }
}
