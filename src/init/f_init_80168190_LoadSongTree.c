#include "../../include/types.h"

/* This function's raw block was auto-merged by splat with the following
   f_init_80168298_InitSoundSystem (mis-detected boundary) - real body ends
   at 0x80168294 (jr ra), see that file's header comment. */

extern void *g_main_80134CF4_SongTrees[];   /* per-kind song tree table, see f_init_80168298_InitSoundSystem */
extern HogArchive *g_main_8011F1A0_Archive;

extern s8 *f_main_800F8258_strcpy(s8 *dst, const s8 *src);
extern s8 *f_main_800F8228_strcat(s8 *dst, const s8 *src);

extern s32 func_80026F4C(void *table, s8 *name);        /* resident-table lookup, returns index or -1 */
extern void func_80026D98(void *table, s32 index, s32 *outSize); /* resident-table entry size */
extern s32 f_main_80026E00_FindResource(HogArchive *archive, const s8 *name, void **out);

extern s32 f_main_800FC13C_BeginVhLoad(VhLoadRecord *rec, s32 findResult, u32 residentSize, s32 a3, s32 a4);
extern s32 func_800FC268(void);   /* guess: polls load completion, nonzero when done */
extern void *func_800FC274(s32 vhSize);  /* guess: finalizes load, returns song tree pointer */

/* guess: loads the "<name>.VB"/"<name>.VH" sound-bank pair for song-tree
   slot `kind`: looks up the resident-table entry (from `table`, the SBNK
   blob returned by f_main_80026C8C_LoadResource) for "<name>.VB" to get its
   size, resolves "<name>.VH" via the archive-wide resource table, then
   streams and finalizes the song tree, storing the result in
   g_main_80134CF4_SongTrees[kind]. `unused` (original 3rd arg) is never
   read. */
void f_init_80168190_LoadSongTree(s32 kind, const s8 *name, void *unused, void *table) {
    s8 path[16];
    VhLoadRecord *vhResult;
    s32 findResult;
    s32 residentSize;

    g_main_80134CF4_SongTrees[kind] = 0;

    f_main_800F8258_strcpy(path, name);
    f_main_800F8228_strcat(path, (const s8 *) ".VB");
    func_80026D98(table, func_80026F4C(table, path), &residentSize);
    f_main_80026E00_FindResource((HogArchive *) table, path, (void **) &findResult);

    f_main_800F8258_strcpy(path, name);
    f_main_800F8228_strcat(path, (const s8 *) ".VH");
    f_main_80026E00_FindResource(g_main_8011F1A0_Archive, path, (void **) &vhResult);

    f_main_800FC13C_BeginVhLoad(vhResult, findResult, residentSize, 0, 0);
    while (func_800FC268() == 0)
        ;
    func_800FC274((s32) PTR_U32(vhResult));

    g_main_80134CF4_SongTrees[kind] = vhResult;
}
