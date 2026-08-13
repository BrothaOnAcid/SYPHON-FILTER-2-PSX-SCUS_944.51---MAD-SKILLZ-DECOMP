#include "../../include/types.h"

/* This function occupies 0x80168298-0x80168648, a range that splat merged
   into the preceding f_init_80168190_LoadSongTree's "nonmatching" block (mis-detected
   boundary - f_init_80168190_LoadSongTree correctly ends at 0x80168294/jr
   ra, and this function starts cleanly right after with its own prologue).
   Determined by manual inspection, not by re-splitting the yaml. */

extern u16 g_main_8012B02C_LevelId;
extern HogArchive *g_main_8011F1A0_Archive;   /* guess: resource archive used for sound bank lookups */
extern void *g_main_8011F904_SbnkTable;       /* guess: loaded SBNK table/array base */
extern MmidObj *g_main_8011F8FC_CurrentMmid;
extern MmidObj *g_main_8011F8F0_QueuedMmid;
extern u8 g_main_8011F1A8_ResetPending;
extern u8 g_main_8011F1A7_unk;
extern s32 g_main_8011F1AC_unk;
extern s32 g_main_8011F1B0_ActiveSoundCount;
extern s32 g_main_8011F1BC_unk;
extern s32 g_main_8011F1C0_unk;
extern u8 g_main_8011F650_unk;
extern u8 g_main_8011F6D8_unk;
extern SoundQueueEntry g_main_80134D4C_SoundQueue[8];
extern RateLimitRule g_main_80134E2C_RateLimits[5];
extern u8 g_main_8011F1A4_unk;   /* byte, guess: config value read into a rate-limit-adjacent call */
extern u8 g_main_8011F1A5_unk;
extern u8 g_main_8011F1A6_unk;
extern u8 g_main_8011F90C_unk;

/* --- libc-style externs (not yet decompiled) --- */
extern s32 f_main_8010008C_SetAudioMode(s32 mode);
extern void f_main_801001C4_SetVolumeGated(s16 volL, s16 volR);
extern s32 f_main_80026C8C_LoadResource(s8 *path, void *arg1, void **outPtr);
extern void f_init_80168190_LoadSongTree(s32 kind, const s8 *name, void *unused, HogToc **table);
extern void f_main_80025B3C_SetContextPtr(void *ptr);
extern s32 f_main_80026E00_FindResource(HogArchive *archive, const s8 *name, void **out);
extern s32 f_main_800FC728_InitMmidNode(MmidObj *obj);
extern void f_main_800FC3F0_CompileEventLists(void);
extern void f_main_800FBAE4_SetChannelParam(s32 index, s32 value);
extern void f_main_800FBBDC_ApplyConfigFlag(s32 value);
extern void f_main_8008E448_SetAudioFlag(s32 a0);
extern void f_main_8008E14C_ResetAudioSlots(void);

/* Master sound-system initializer, called once per level load from
   f_main_8002A338_LoadLevel right after loading the per-level SLF.RFF
   resource. No-ops entirely if `path` is NULL; `unused1/2/3` don't appear
   to be read anywhere in this function despite the caller passing real
   values. */
void f_init_80168298_InitSoundSystem(void *path, s32 unused1, void *unused2, s32 unused3) {
    s32 mode;
    s32 level;
    HogToc **result;
    HogToc *sbnkPtr;
    s32 i;

    (void) unused1;
    (void) unused2;
    (void) unused3;

    if (path == 0)
        return;

    /* Pick a sound-quality/mode constant based on the current level. */
    level = (s16) g_main_8012B02C_LevelId;
    if (level == 0x12 || level == 0x13 || level == 0xF || level == 4 || level == 0xB ||
        (u32) (level - 0x17) < 0x14)
        mode = 2;
    else
        mode = 3;
    f_main_8010008C_SetAudioMode(mode);

    f_main_801001C4_SetVolumeGated(0, 0);

    /* Resolve the per-level SBNK table via `path`, then load 4 SBNK-backed
       song trees (kinds 3, 0, 1, 2) into g_main_80134CF4_SongTrees, using
       f_init_80168190_LoadSongTree. */
    f_main_80026C8C_LoadResource((s8 *) path, 0, (void **) &result);
    sbnkPtr = *result;
    g_main_8011F904_SbnkTable =
        (u8 *) sbnkPtr + (sbnkPtr->unk00 << 2);
    f_init_80168190_LoadSongTree(3, (const s8 *) "MUSIC", 0, result);
    f_init_80168190_LoadSongTree(0, (const s8 *) "AM", 0, result);
    f_init_80168190_LoadSongTree(1, (const s8 *) "PL", 0, result);
    f_init_80168190_LoadSongTree(2, (const s8 *) "RC", 0, result);

    f_main_80025B3C_SetContextPtr(g_main_8011F904_SbnkTable);

    g_main_8011F8FC_CurrentMmid = 0;
    if (f_main_80026E00_FindResource(g_main_8011F1A0_Archive, (const s8 *) "MI.MMD",
                                      (void **) &g_main_8011F8FC_CurrentMmid) != 0)
        f_main_800FC728_InitMmidNode(g_main_8011F8FC_CurrentMmid);

    g_main_8011F8F0_QueuedMmid = 0;
    {
        s32 idx = *(s32 *) g_main_8011F904_SbnkTable;
        if (idx != 0) {
            g_main_8011F8F0_QueuedMmid = (MmidObj *) ((u8 *) g_main_8011F904_SbnkTable + (idx << 2));
            f_main_800FC728_InitMmidNode(g_main_8011F8F0_QueuedMmid);
        }
    }

    f_main_800FC3F0_CompileEventLists();

    g_main_8011F1A8_ResetPending = 1;
    g_main_8011F1A7_unk = 0;
    g_main_8011F1AC_unk = 0;
    g_main_8011F1B0_ActiveSoundCount = 0;
    g_main_8011F1BC_unk = 0;
    g_main_8011F1C0_unk = -1;
    g_main_8011F650_unk = 0;
    g_main_8011F6D8_unk = 0;

    for (i = 0; i < 8; i++) {
        g_main_80134D4C_SoundQueue[i].id = -1;
        g_main_80134D4C_SoundQueue[i].subId = -1;
        g_main_80134D4C_SoundQueue[i].ctx = 0;
        g_main_80134D4C_SoundQueue[i].active = 0;
    }

    for (i = 0; i < 5; i++) {
        g_main_80134E2C_RateLimits[i].category = -1;
        g_main_80134E2C_RateLimits[i].idMin = -1;
        g_main_80134E2C_RateLimits[i].idMax = -1;
        g_main_80134E2C_RateLimits[i].lastTime = 0;
        g_main_80134E2C_RateLimits[i].cooldown = 0;
    }

    /* Fixed default rate-limit rules (game-tuning data). */
    g_main_80134E2C_RateLimits[0].category = 1;
    g_main_80134E2C_RateLimits[0].idMin = 0x17;
    g_main_80134E2C_RateLimits[0].idMax = 0x1B;
    g_main_80134E2C_RateLimits[0].cooldown = 0xA;

    g_main_80134E2C_RateLimits[1].category = 2;
    g_main_80134E2C_RateLimits[1].idMin = 0;
    g_main_80134E2C_RateLimits[1].cooldown = 5;

    g_main_80134E2C_RateLimits[2].category = 2;
    g_main_80134E2C_RateLimits[2].idMin = 0xC;
    g_main_80134E2C_RateLimits[2].idMax = 0xF;
    g_main_80134E2C_RateLimits[2].cooldown = 5;

    g_main_80134E2C_RateLimits[3].category = 2;
    g_main_80134E2C_RateLimits[3].idMin = 0x25;
    g_main_80134E2C_RateLimits[3].idMax = 0x29;
    g_main_80134E2C_RateLimits[3].cooldown = 5;

    f_main_800FBAE4_SetChannelParam(0, g_main_8011F1A4_unk);
    f_main_800FBAE4_SetChannelParam(1, g_main_8011F1A5_unk);
    f_main_800FBAE4_SetChannelParam(2, g_main_8011F1A6_unk);

    f_main_800FBBDC_ApplyConfigFlag(g_main_8011F90C_unk);

    f_main_8008E448_SetAudioFlag(1);
    f_main_8008E14C_ResetAudioSlots();
}
