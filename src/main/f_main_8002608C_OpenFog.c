#include "../../include/types.h"

extern FileHandle *g_main_8011EE64_Handle;
extern void *g_main_8011EE68_FogDesc;  /* nonzero = a FOG archive is open;
                                           set here to &D_80126058 */
extern u8 g_main_80126058_FogDesc[0x240];  /* guess: FOG archive descriptor;
    f_main_80010750_CopyWordsUnrolled below copies 0x90 WORDS (0x240 bytes)
    into it from the header buffer, so it's at least that big; +0x8 is a
    stream/decompression context */

/* --- libc-style externs (not yet decompiled) --- */
extern void f_main_8002605C_CloseIfOpen(void);
extern s32 func_80026234(void *pathBuf, FileHandle **outHandle);  /* likely open(pathBuf) */
extern s32 func_80026414(FileHandle *handle, void *buf, s32 size, void *outInfo); /* likely read(handle, buf, size) */
extern void f_main_80010750_CopyWordsUnrolled(void *dst, void *src, s32 count);
extern s32 func_800F836C(FileHandle *handle);

/* Opens the FOG archive at `path`: closes any previously-open archive,
   opens the file, reads a 0x800-byte header into a stack buffer, copies
   the first 0x240 bytes (0x90 words) of it into the global FOG descriptor
   (g_main_80126058_FogDesc), marks the archive open
   (g_main_8011EE68_FogDesc = &g_main_80126058_FogDesc), and initializes a
   stream/decompression context via func_800F836C(handle) into
   descriptor+0x8. Returns 1 on success, 0 if the file couldn't be
   opened. */
s32 f_main_8002608C_OpenFog(void *path) {
    u8 header[0x800];
    s32 readInfo;

    f_main_8002605C_CloseIfOpen();

    if (func_80026234(path, &g_main_8011EE64_Handle) != 0)
        return 0;

    func_80026414(g_main_8011EE64_Handle, header, 0x800, &readInfo);

    f_main_80010750_CopyWordsUnrolled(g_main_80126058_FogDesc, header, 0x90);

    g_main_8011EE68_FogDesc = g_main_80126058_FogDesc;
    *(s32 *) (g_main_80126058_FogDesc + 0x8) = func_800F836C(g_main_8011EE64_Handle);

    return 1;
}
