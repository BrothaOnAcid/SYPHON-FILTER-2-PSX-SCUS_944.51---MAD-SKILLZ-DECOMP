#include "../../include/types.h"

/* Guessed: 2-arg fixed-point math helper (libc-style, no asm entry here).
   Used all over the actor code; returns a single s32. */
extern s32 func_800F02AC(s32 a0, s32 a1);

/* Derives a rotation value: out[0] = -func_800F02AC(vec[1], xz-mag). Only
   out[0] is written; out[1..3] are pre-seeded by the caller. */
extern void f_main_8002789C_DeriveRot(s32 *vec, s32 *out);

/* Normalizes a 3-word vector: func_80010B14(src, dst). */
extern void func_80010B14(s32 *src, s32 *dst);

/* Recomputed whenever the actor is taking recoil (dispatched by
   f_main_80068C38_UpdateActors once the recoil angle exceeds a threshold).
   Builds a rotation from the current angles (X and Z negated) into core
   +0xC0..0xCC, and writes the (normalized, when angY != 0) direction vector
   into the active action +0x18..0x24. */
void f_main_8006C730_UpdateRecoil(ActorNode *node) {
    AngBlock *ang;
    ActorCore *core;
    ActorAction *act;
    s32 v[4];
    s32 out[4];
    s32 r;

    ang = node->anim->unk0C;
    v[0] = -ang->angX;
    v[1] = ang->angY;
    v[2] = -ang->angZ;

    r = func_800F02AC(v[0], v[2]);
    f_main_8002789C_DeriveRot(&v[0], &out[0]);
    out[1] = r;
    out[2] = 0;

    core = node->core;
    core->unkC0 = out[0];
    core->unkC4 = out[1];
    core->unkC8 = out[2];
    core->unkCC = out[3];

    act = core->act;
    if (v[1] != 0) {
        func_80010B14(&v[0], &v[0]);
        v[1] = 0;
    }
    act->unk18 = v[0];
    act->unk1C = v[1];
    act->unk20 = v[2];
    act->unk24 = v[3];
}
