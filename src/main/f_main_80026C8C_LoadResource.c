#include "../../include/types.h"

extern s8 D_8011EE70;   /* sentinel address - callers pass &D_8011EE70 for "no path" */

extern s32 func_80026234(void *pathBuf, FileHandle **outHandle);  /* likely open(pathBuf) */
extern s32 f_main_80026A60_StreamResource(FileHandle *handle, void **out, void *buf);

/* Opens `path` and hands the resulting file handle to
   f_main_80026A60_StreamResource to stream it whole into memory. Returns 1
   without doing anything if `path` is NULL, is the &D_8011EE70 sentinel,
   or `outPtr` is NULL. `arg1` is passed straight through into
   f_main_80026A60_StreamResource's dead `$a1` register slot - genuinely
   unused, confirmed by that function's decompilation. */
s32 f_main_80026C8C_LoadResource(s8 *path, void *arg1, void **outPtr) {
    FileHandle *h;
    s32 r;

    (void) arg1;

    if (path == 0 || path == (s8 *) &D_8011EE70 || outPtr == 0) {
        return 1;
    }
    *outPtr = 0;

    r = func_80026234(path, &h);
    if (r != 0) {
        return r;
    }
    return f_main_80026A60_StreamResource(h, outPtr, 0);
}
