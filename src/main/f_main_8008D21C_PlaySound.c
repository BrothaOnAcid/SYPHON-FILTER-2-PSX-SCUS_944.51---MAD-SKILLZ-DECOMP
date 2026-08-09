#include "../../include/types.h"

extern u16 g_main_8012B02C_LevelId;              /* guess: current level/map id */
extern SongTree *g_main_80134D00_AmbientTree;    /* category-3's dedicated tree */
extern SongTree *g_main_80134CF4_SongTrees[];    /* per-category tree, indexed by `category` */
extern RateLimitRule g_main_80134E2C_RateLimits[5];
extern u32 g_main_80134E30_RateLastTime[5];      /* parallel to RateLimits, written on match */
extern s32 g_main_8011F668_unk;                  /* guess: tick counter used for rate limiting */
extern SoundQueueEntry g_main_80134D4C_SoundQueue[8];
extern s32 g_main_8011F1B0_ActiveSoundCount;     /* gp-rel */

/* --- libc-style externs (not yet decompiled) --- */
extern s32 func_800FA680(SongTree *tree, s32 idx);
extern void func_8008CDD0(s32 category, s32 id, void *ctx, void *posArg,
                           s16 *outOv1, s16 *outOv2);
extern s32 f_main_800F9F68_PlaySongNode(SongTree *tree, s32 idx, s32 ov1, s32 ov2);

/* Plays sound `id` in `category`. `ctx` is an opaque caller context
   (e.g. AmmoUser*, see f_main_800644AC_HandleAmmo); `posArg`, if non-NULL,
   points to a 3-word position used for looping/positional sounds that need
   continuous tracking.

   category 3 is a special "ambient" path: suppressed on two specific
   levels (g_main_8012B02C_LevelId == 0x15 or 0x16), otherwise dispatched
   straight to g_main_80134D00_AmbientTree with fixed "keep as-is"
   overrides (-2, -2), bypassing rate limiting and queue tracking entirely.

   Other categories: checked against g_main_80134E2C_RateLimits (dropped if
   still on cooldown), then resolved via func_8008CDD0 into a pair of
   pitch/vol overrides, played through
   g_main_80134CF4_SongTrees[category] via f_main_800F9F68_PlaySongNode,
   and - if func_800FA680 says the result needs tracking - registered into
   the 8-slot g_main_80134D4C_SoundQueue for later positional updates. */
s32 f_main_8008D21C_PlaySound(s32 category, s32 id, void *ctx, void *posArg) {
    s32 i;
    s32 result;
    s16 ov1;
    s16 ov2;
    SongTree *tree;
    s32 subId;

    if (category == 3) {
        if ((u16) (g_main_8012B02C_LevelId - 0x15) < 2)
            return -1;
        return f_main_800F9F68_PlaySongNode(g_main_80134D00_AmbientTree, id, -2, -2);
    }

    for (i = 0; i < 5; i++) {
        RateLimitRule *rule = &g_main_80134E2C_RateLimits[i];

        if (rule->category != category)
            continue;
        if (id < rule->idMin)
            continue;
        if (rule->idMax < id)
            continue;

        if (rule->cooldown < (u32) (g_main_8011F668_unk - rule->lastTime)) {
            g_main_80134E30_RateLastTime[i] = g_main_8011F668_unk;
            break;
        }
        return -1;
    }

    func_8008CDD0(category, id, ctx, posArg, &ov1, &ov2);
    if (ov1 == 0) {
        if (func_800FA680(g_main_80134CF4_SongTrees[category], id) == 0)
            return -1;
    }

    tree = g_main_80134CF4_SongTrees[category];
    result = f_main_800F9F68_PlaySongNode(tree, id, ov1, ov2);
    subId = (s16) result;
    if (subId == -1)
        subId = id;

    if (ctx == 0 && posArg == 0)
        return (s16) subId;

    if (func_800FA680(g_main_80134CF4_SongTrees[category], subId) == 0)
        return (s16) subId;

    for (i = 0; i < 8; i++) {
        SoundQueueEntry *slot = &g_main_80134D4C_SoundQueue[i];

        if (slot->id != -1)
            continue;

        slot->id = category;
        slot->subId = subId;
        slot->ctx = ctx;
        if (posArg != 0) {
            s32 *src = (s32 *) posArg;
            slot->pos[0] = src[0];
            slot->pos[1] = src[1];
            slot->pos[2] = src[2];
            slot->active = 1;
        }
        g_main_8011F1B0_ActiveSoundCount++;
        break;
    }

    return (s16) subId;
}
