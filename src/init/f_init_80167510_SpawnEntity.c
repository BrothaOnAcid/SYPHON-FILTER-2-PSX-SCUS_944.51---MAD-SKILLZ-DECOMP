#include "../../include/types.h"

extern s32 D_8010C874[];   /* not yet decompiled */
extern u16 D_8011EC70;
extern s16 D_8012B02C;
extern u8 D_80158D64[];    /* not yet decompiled */
extern u16 D_80168AA0;     /* not yet decompiled: running id counter */

extern void func_8001543C(s32 a0, u16 *out);                                           /* not yet decompiled */
extern s32 func_8010007C(s32 a0, void *a1, s32 a2);                                     /* not yet decompiled */
extern void func_80015960(s32 a0, s32 a1, u16 a2, s32 a3, s32 a4, s32 a5, s32 a6, void **out); /* not yet decompiled */
extern void func_80024908(s32 a0, s32 a1, s32 a2, void *a3);                             /* not yet decompiled */

/* Resolves a "priority" value (2, or 0xB as a fallback) then spawns an
   entity via func_80015960. The fallback (0xB) kicks in when
   D_8011EC70 < 2, or the current stage (D_8012B02C) is 0xE, or
   func_8010007C(D_8010C874[D_8012B02C], D_80158D64, 5) fails.

   The spawned entity (returned by func_80015960) then gets a fresh id
   stamped into its +0x14 from the running D_80168AA0 counter, its +0x6 flag
   word gets bit 8 set, and it's registered via func_80024908 before being
   returned to the caller. */
void *f_init_80167510_SpawnEntity(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5) {
    u16 sp20;
    void *entity;
    s32 priority = 2;
    u16 id;
    u16 *entityFlags;

    func_8001543C(arg2, &sp20);

    if (D_8011EC70 < 2 || D_8012B02C == 0xE ||
        func_8010007C(D_8010C874[D_8012B02C], D_80158D64, 5) == 0) {
        priority = 0xB;
    }

    func_80015960(0, arg0, sp20, arg3, arg4, priority, arg5, &entity);

    id = D_80168AA0;
    D_80168AA0 = id + 1;
    *(s32 *) ((u8 *) entity + 0x14) = (s16) id;

    entityFlags = (u16 *) ((u8 *) entity + 0x6);
    *entityFlags |= 8;

    func_80024908(*(s32 *) entity, 0, arg1, entity);

    return entity;
}
