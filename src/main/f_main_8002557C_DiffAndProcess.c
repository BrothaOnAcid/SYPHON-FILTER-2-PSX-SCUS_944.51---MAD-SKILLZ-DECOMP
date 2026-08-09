#include "../../include/types.h"

extern s32 func_80025004(s32 p0, s32 p1, VECTOR *out); /* fills *out, returns a status/flag */
extern void func_800253C0(s32 p0, s32 p1, VECTOR *diff, s32 p3);

/* Computes tmp via func_80025004(p0, p1, &tmp); if that reports failure
   (return 0), passes (p2 - tmp) to func_800253C0 along with p3. */
void f_main_8002557C_DiffAndProcess(s32 p0, s32 p1, VECTOR *p2, s32 p3) {
    VECTOR tmp;
    VECTOR diff;
    s32 ok;

    ok = func_80025004(p0, p1, &tmp);

    diff.vx = p2->vx - tmp.vx;
    diff.vy = p2->vy - tmp.vy;
    diff.vz = p2->vz - tmp.vz;

    if (!ok) {
        func_800253C0(p0, p1, &diff, p3);
    }
}
