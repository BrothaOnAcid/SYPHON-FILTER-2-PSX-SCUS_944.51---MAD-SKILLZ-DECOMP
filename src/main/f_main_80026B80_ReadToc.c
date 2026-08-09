#include "../../include/types.h"

extern void *f_main_80025AD0_AllocDown(s32 size);
extern void f_main_80025B3C_SetContextPtr(void *ptr);
extern s32 f_main_8002662C_CloseFile(FileHandle **handlePtr);

/* --- libc-style externs (not yet decompiled) --- */
extern s32 func_80026234(s8 *path, FileHandle **outHandle); /* likely open(path) */
extern s32 func_80026414(FileHandle *handle, void *buf, s32 size, void *outInfo); /* likely read(handle, buf, size) */
extern s32 f_main_800265A8_PrepareRead(FileHandle *handle);

/* Opens `path`, reads a header into an 0x800-byte buffer (same size as
   f_main_8002608C_OpenFog's FOG header), and if the header's declared size
   (+0x10 - same field FOG treats as header/data-start size) is bigger than
   0x800, reallocates a correctly-sized buffer and rereads. Hands the final
   buffer back through `*outToc`. `flags` (2nd param) is read nowhere in the
   body - a dead argument. Returns 1 if `path` is NULL, 4 on open/read
   failure, 0 on success. */
s32 f_main_80026B80_ReadToc(s8 *path, s32 flags, WldToc **outToc) {
    FileHandle *h;
    WldToc *toc;
    s32 sz;
    u8 info[4]; /* guess: read-result out-param, shape unconfirmed */

    toc = f_main_80025AD0_AllocDown(0x800);

    if (path == 0) {
        return 1;
    }

    if (func_80026234(path, &h) != 0) {
        return 4;
    }

    sz = 0x800;
    if (func_80026414(h, toc, 0x800, info) != 0) {
        f_main_8002662C_CloseFile(&h);
        return 4;
    }

    if (toc->dataOffset > 0x800) {
        sz = toc->dataOffset;
        f_main_80025B3C_SetContextPtr(toc);
        toc = f_main_80025AD0_AllocDown(sz);
        if (f_main_800265A8_PrepareRead(h) != 0 || func_80026414(h, toc, sz, info) != 0) {
            f_main_8002662C_CloseFile(&h);
            return 4;
        }
    }

    *outToc = toc;
    f_main_8002662C_CloseFile(&h);
    return 0;
}
