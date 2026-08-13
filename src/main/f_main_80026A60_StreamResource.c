#include "../../include/types.h"

extern s32 f_main_800263C4_GetFileSize(FileHandle *handle, s32 *outSize);
extern void *f_main_80025AD0_AllocDown(s32 size);
extern s32 f_main_8002662C_CloseFile(FileHandle **hp);
extern s32 func_80026414(FileHandle *handle, void *buf, s32 size, void *outInfo); /* likely read(handle, buf, size) */
extern void f_main_80025B3C_SetContextPtr(void *ptr); /* also doubles as "rewind the bump
                                                           allocator's watermark to ptr" -
                                                           i.e. this function's error-path
                                                           "free" of a fresh AllocDown block */

/* Streams the whole contents of `handle` into a buffer and hands it back
   through `*out`. If `buf` is non-NULL it's used as-is (caller-owned);
   otherwise a fresh buffer is bump-allocated (f_main_80025AD0_AllocDown)
   sized to the file (f_main_800263C4_GetFileSize). `handle` is always
   closed before returning (f_main_8002662C_CloseFile) - the caller only
   gets the streamed buffer back, never the handle.

   If `out` is non-NULL: allocates an 8-byte wrapper the first time
   *out is NULL and stores the buffer pointer into *(*out) - this is the
   `HogToc **out` pattern used by f_main_80026C8C_LoadResource /
   f_init_80168298_InitSoundSystem (see HogToc's doc comment in types.h):
   *out ends up pointing at a small heap wrapper whose first field is the
   raw streamed file bytes, i.e. `**out` IS the loaded file's own HogToc
   header verbatim - this function does no parsing of its own, it's a
   generic "read whole file into memory" helper. */
s32 f_main_80026A60_StreamResource(FileHandle *handle, void **out, void *buf) {
    s32 size;
    s32 err;
    s32 readInfo;

    err = f_main_800263C4_GetFileSize(handle, &size);
    if (err != 0) {
        f_main_8002662C_CloseFile(&handle);
        return err;
    }

    if (buf == 0) {
        buf = f_main_80025AD0_AllocDown(size);
        if (buf == 0)
            return 3;
    }

    err = func_80026414(handle, buf, size, &readInfo);
    if (err != 0) {
        f_main_8002662C_CloseFile(&handle);
        goto fail;
    }

    err = f_main_8002662C_CloseFile(&handle);
    if (err != 0) {
fail:
        f_main_80025B3C_SetContextPtr(buf);
        return err;
    }

    if (out != 0) {
        if (*out == 0) {
            *out = f_main_80025AD0_AllocDown(8);
            if (*out == 0) {
                f_main_80025B3C_SetContextPtr(buf);
                return 3;
            }
        }
        *(void **) *out = buf;
    }

    return 0;
}
