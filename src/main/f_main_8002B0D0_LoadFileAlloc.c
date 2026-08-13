#include "../../include/types.h"

extern s32 g_main_8011EEC4_ResidentTableFlag; /* nonzero once the requested file is already resident */
extern s32 D_8010C874[];
extern s16 D_8012B02C;
extern u8 D_80109058[]; /* not yet decompiled: sprintf format string, one %s substitution */
extern u8 D_80109068[]; /* not yet decompiled: sprintf format string, two substitutions */

extern s32 f_main_8002B3D8_GetResidentFile(s8 *name, void **outPtr, u8 mode);
extern s8 *f_main_800F8258_strcpy(s8 *dst, const s8 *src);
extern void func_800F045C(void *dst, u8 *fmt, s32 a, s32 b); /* not yet decompiled: sprintf-ish */
extern s32 func_80026234(void *pathBuf, FileHandle **outHandle);  /* likely open(pathBuf) */
extern s32 f_main_800263C4_GetFileSize(FileHandle *handle, s32 *outSize);
extern void *func_80025AD0(s32 size);
extern s32 func_80026414(FileHandle *handle, void *buf, s32 size, void *outInfo); /* likely read(handle, buf, size) */
extern s32 f_main_8002662C_CloseFile(FileHandle **handlePtr);
extern void func_8002BCF4(void); /* not yet decompiled */

/* Loads `name` into a freshly-allocated (func_80025AD0) buffer, retrying up
   to 6 times. Tries the name as-is first; if that fails to open, retries
   with two formatted variants (D_80109058: name alone; D_80109068: current
   level id (D_8012B02C -> D_8010C874) + name), calling func_8002BCF4
   (probably a "disc swap" prompt) and looping if all three fail.

   `size` selects how much to read/allocate:
     < 0  - allocate max(|size|, real file size)
     == 0 - allocate the real file size
     > 0  - use exactly `size`, WITHOUT allocating - *outPtr is read into
            as-is (caller-owned buffer), matching the asm's direct jump
            around the func_80025AD0 call in this case.

   If g_main_8011EEC4_ResidentTableFlag is set and `size` is 0, first tries
   f_main_8002B3D8_GetResidentFile once (not retried) and returns
   immediately if that succeeds.

   Returns the loaded size, or 0 if every attempt failed. NOTE: matches a
   real asm quirk - a failed allocation (func_80025AD0 returning NULL)
   re-enters the retry-count check without first re-validating a fresh
   attempt, effectively burning two attempts in a row; a failed read only
   burns one. Preserved via goto rather than smoothed over. */
u32 f_main_8002B0D0_LoadFileAlloc(s8 *name, void **outPtr, s32 size) {
    s8 path[56];
    FileHandle *h;
    s32 fileSize;
    s32 readInfo;
    s32 i = 0;

    if (g_main_8011EEC4_ResidentTableFlag != 0 && size == 0) {
        u32 r = f_main_8002B3D8_GetResidentFile(name, outPtr, 1);
        if (r != 0) {
            return r;
        }
    }

retry:
    if (i >= 6) {
        return 0;
    }
    i++;

    f_main_800F8258_strcpy(path, name);
    if (func_80026234(path, &h) == 0) {
        goto opened;
    }
    func_800F045C(path, D_80109058, (s32) (u64) name, 0);
    if (func_80026234(path, &h) == 0) {
        goto opened;
    }
    func_800F045C(path, D_80109068, D_8010C874[D_8012B02C], (s32) (u64) name);
    if (func_80026234(path, &h) == 0) {
        goto opened;
    }

    func_8002BCF4();
    goto retry;

opened:
    if (size < 0) {
        s32 minAbs = -size;
        f_main_800263C4_GetFileSize(h, &fileSize);
        if ((u32) fileSize < (u32) minAbs) {
            fileSize = minAbs;
        }
        *outPtr = func_80025AD0(fileSize);
    } else if (size == 0) {
        f_main_800263C4_GetFileSize(h, &fileSize);
        *outPtr = func_80025AD0(fileSize);
    } else {
        fileSize = size;
    }

    if (*outPtr == 0) {
        f_main_8002662C_CloseFile(&h);
        goto retry;
    }

    if (func_80026414(h, *outPtr, fileSize, &readInfo) != 0) {
        f_main_8002662C_CloseFile(&h);
        *outPtr = 0;
        i = i - 1; /* matches L8002B11C: re-enter via the un-incremented count */
        goto retry;
    }

    f_main_8002662C_CloseFile(&h);
    return (u32) fileSize;
}
