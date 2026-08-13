#include "../../include/types.h"

/* This function's raw block was auto-merged by splat with the following
   f_init_80168298_InitSoundSystem (mis-detected boundary) - real body ends
   at 0x80168294 (jr ra), see that file's header comment. */

extern void *g_main_80134CF4_SongTrees[];   /* per-kind song tree table, see f_init_80168298_InitSoundSystem */
extern HogArchive *g_main_8011F1A0_Archive;

extern s8 *f_main_800F8258_strcpy(s8 *dst, const s8 *src);
extern s8 *f_main_800F8228_strcat(s8 *dst, const s8 *src);

extern s32 f_main_80026F4C_FindNamedIndex(HogToc **toc, const s8 *name);
extern s32 f_main_80026D98_GetEntrySize(HogToc **toc, s32 index, s32 *outSize);
extern s32 f_main_80026E00_FindResource(HogArchive *archive, const s8 *name, void **out);

extern s32 f_main_800FC13C_BeginVhLoad(VhLoadRecord *rec, s32 findResult, u32 residentSize, s32 a3, s32 a4);
extern s32 f_main_800FC268_IsVhLoadDone(void);
extern void f_main_800FC274_FinalizeVhLoad(SongTree *rec);

/* Loads the "<name>.VB"/"<name>.VH" sound-bank pair for song-tree slot
   `kind`: looks up "<name>.VB" in `table` (the level's SLF.RFF-derived
   HOG archive, from f_main_80026C8C_LoadResource) to get its size,
   resolves "<name>.VB"'s data pointer via the same archive, then resolves
   "<name>.VH" via the separate archive-wide g_main_8011F1A0_Archive, and
   streams+finalizes the song tree, storing the result in
   g_main_80134CF4_SongTrees[kind]. `unused` (original 3rd arg) is never
   read.

   CAVEAT (2026-08-12): `table`'s HogToc format is now confirmed byte-exact
   against a real sample (toolz/test_fog's arena13.fog -> VLF.RFF - see
   HogToc's doc comment in types.h and toolz/AUDIO_SEQ_TODO.md), but that
   sample's archive only has 5 entries ("AM.VH"/"MI.MMD"/"MUSIC.VH"/
   "PL.VH"/"RC.VH") - no "<name>.VB" entries, meaning the lookups below
   would fail (return early, leaving `residentSize`/`findResult`
   uninitialized) if `table` really were that exact archive. Either the
   real per-level `table` is a different/bigger archive that does carry
   ".VB" entries, or there's more to this call than reverse-engineered so
   far - unresolved, see AUDIO_SEQ_TODO.md. */
void f_init_80168190_LoadSongTree(s32 kind, const s8 *name, void *unused, HogToc **table) {
    s8 path[16];
    VhLoadRecord *vhResult;
    s32 findResult;
    s32 residentSize;

    g_main_80134CF4_SongTrees[kind] = 0;

    f_main_800F8258_strcpy(path, name);
    f_main_800F8228_strcat(path, (const s8 *) ".VB");
    f_main_80026D98_GetEntrySize(table, f_main_80026F4C_FindNamedIndex(table, path), &residentSize);
    f_main_80026E00_FindResource((HogArchive *) table, path, (void **) &findResult);

    f_main_800F8258_strcpy(path, name);
    f_main_800F8228_strcat(path, (const s8 *) ".VH");
    f_main_80026E00_FindResource(g_main_8011F1A0_Archive, path, (void **) &vhResult);

    f_main_800FC13C_BeginVhLoad(vhResult, findResult, residentSize, 0, 0);
    while (f_main_800FC268_IsVhLoadDone() == 0)
        ;
    f_main_800FC274_FinalizeVhLoad(vhResult);

    g_main_80134CF4_SongTrees[kind] = vhResult;
}
