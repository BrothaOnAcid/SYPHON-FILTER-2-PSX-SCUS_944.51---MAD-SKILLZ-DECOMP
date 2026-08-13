#include "../../include/types.h"

extern u32 g_main_8011EEBC_Template[2];      /* guess: 8-byte template copied into a local buffer */
extern u16 g_main_8012B02C_LevelId;          /* current level/map id */
extern u8 g_main_8011F624_Flag;              /* guess: previously "debug/cheat" flag; here set
    from (level < 0x17), so more likely a level-group/difficulty-tier flag that also happens
    to gate the ammo-cheat behavior elsewhere - not reconciled */
extern s8 *g_main_8010C874_LevelNames[];     /* level name string table, indexed by level id */
extern s32 g_main_8011F61C_unk;
extern void *g_main_8011EF00_unk;
extern void *g_main_8011EF08_unk;
extern u8 g_main_80158878_ArenaEnd[];        /* overlay memory arena end; also the fixed
                                                 load address for INIT.OVL */
extern s8 g_main_80109030_InitOvlName[];     /* "INIT.OVL" */

/* --- libc-style externs (not yet decompiled) --- */
extern void func_800F1508(void *a0, s32 a1, s32 a2, s32 a3);
extern void func_800F1384(s32 a0);
extern void func_800F045C(void *pathBuf, const char *fmt, ...); /* sprintf-like */
extern s32 f_main_8002608C_OpenFog(void *path);  /* decompiled */
extern u32 f_main_8002B4C4_LoadFile(s8 *name, void *arg1, s32 mode);
extern void func_80025B48(void *ptr);
extern void f_init_80168298_InitSoundSystem(void *path, s32 a1, void *outSize, s32 a3);
extern void *func_80025B54(void);
extern void f_main_8002BC44_PushState(s32 flag);
extern void f_main_8002BDC0_SetSoundState(u8 state);

/* Not yet decompiled, but this is a direct call into the INIT.OVL overlay
   just loaded by f_main_8002B4C4_LoadFile below - confirms
   g_main_80158878_ArenaEnd is INIT.OVL's fixed load address. */
extern void f_init_8015CF84_ClearOverlayTail(u32 *ptr);

/* Loads level `level`'s FOG archive and the INIT.OVL overlay, then a
   per-level SLF.RFF resource. `restart`/`fromSave` (guessed) select the
   final f_main_8002BC44_PushState(0xC) vs f_main_8002BDC0_SetSoundState(4) call.

   `level` is clamped to [0, 0x2A]; if out of range, level is reset to 0.
   g_main_8011F624_Flag is set based on whether the (possibly clamped)
   level is below 0x17. */
void f_main_8002A338_LoadLevel(s32 level, s32 restart, s32 fromSave) {
    u32 tmp[2];
    s8 path[0x48];   /* sp+0x10..sp+0x58: stack buffer, reused per section */
    s8 path2[0x20];  /* sp+0x18..sp+0x38 in the second half of the function */
    s32 outSize;
    s32 isLowLevel;

    tmp[0] = g_main_8011EEBC_Template[0];
    tmp[1] = g_main_8011EEBC_Template[1];
    func_800F1508(tmp, 0, 0, 0);
    func_800F1384(0);

    if ((u32) level >= 0x2B)
        level = 0;
    isLowLevel = level < 0x17;

    g_main_8011F61C_unk = 0;
    g_main_8012B02C_LevelId = (u16) level;

    g_main_8011F624_Flag = isLowLevel ? 0 : 1;

    func_800F045C(path, "\\FOG\\%s.FOG;1", g_main_8010C874_LevelNames[g_main_8012B02C_LevelId]);
    f_main_8002608C_OpenFog(path);

    f_main_8002B4C4_LoadFile(g_main_80109030_InitOvlName, g_main_80158878_ArenaEnd, 0);

    func_80025B48(g_main_8011EF00_unk);

    func_800F045C(path2, "\\%s\\SLF.RFF;1", g_main_8010C874_LevelNames[g_main_8012B02C_LevelId]);
    f_init_80168298_InitSoundSystem(path2, 0, &outSize, 0);
    g_main_8011EF08_unk = func_80025B54();

    f_init_8015CF84_ClearOverlayTail((u32 *) g_main_8011EF08_unk);

    if (fromSave == 0) {
        f_main_8002BC44_PushState(0xC);
    } else {
        f_main_8002BDC0_SetSoundState(4);
    }
    g_main_8011F61C_unk = restart < 1;
}
