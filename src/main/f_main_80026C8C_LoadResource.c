#include "../../include/types.h"

extern s8 D_8011EE70;   /* sentinel address - callers pass &D_8011EE70 for "no path" */

extern s32 func_80026234(void *pathBuf, FileHandle **outHandle);  /* likely open(pathBuf) */
extern s32 func_80026A60(FileHandle *handle, void *arg1, void **outPtr, s32 a3, s32 a4);

/* guess: opens `path` and hands the resulting file handle to func_80026A60
   to actually load it. Returns 1 without doing anything if `path` is NULL,
   is the &D_8011EE70 sentinel, or `outPtr` is NULL. */
s32 f_main_80026C8C_LoadResource(s8 *path, void *arg1, void **outPtr) {
    FileHandle *h;
    s32 r;

    if (path == 0 || path == (s8 *) &D_8011EE70 || outPtr == 0) {
        return 1;
    }
    *outPtr = 0;

    r = func_80026234(path, &h);
    if (r != 0) {
        return r;
    }
    return func_80026A60(h, arg1, outPtr, 0, 0);
}
