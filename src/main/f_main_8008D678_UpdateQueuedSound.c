#include "../../include/types.h"

extern SongTree *g_main_80134CF4_SongTrees[];

extern void func_8008CDD0(s32 category, s32 id, void *ctx, void *posArg,
                           s16 *outOv1, s16 *outOv2);
extern void func_800FAF08(SongTree *tree, s32 id, s32 ov2, s32 a3, s32 a4, s32 a5);
extern void func_800FA8D8(SongTree *tree, s32 id, s32 ov1, s32 a3);

/* Per-frame refresh of a live positional/looping sound's pitch/volume,
   called once per active g_main_80134D4C_SoundQueue entry from
   f_main_8008E990_ProcessSoundQueue (with `ov1Override`/`ov2Override`
   passed as -1, i.e. "recompute, don't override"). Recomputes the
   category's pitch/volume overrides via func_8008CDD0 (same helper used
   by f_main_8008D21C_PlaySound), lets the caller stomp either value if it
   passed something other than -1, then pushes both values down through
   func_800FAF08/func_800FA8D8 onto the song tree node for `id`. */
void f_main_8008D678_UpdateQueuedSound(s32 category, s32 id, void *ctx, void *posArg,
                                        s16 ov1Override, s16 ov2Override) {
    SongTree *tree = g_main_80134CF4_SongTrees[category];
    s16 ov1;
    s16 ov2;

    func_8008CDD0(category, id, ctx, posArg, &ov1, &ov2);

    if (ov1Override != -1)
        ov1 = ov1Override;
    if (ov2Override != -1)
        ov2 = ov2Override;

    func_800FAF08(tree, id, ov2, 0, 0x1E, 4);
    func_800FA8D8(tree, id, ov1, -2);
}
