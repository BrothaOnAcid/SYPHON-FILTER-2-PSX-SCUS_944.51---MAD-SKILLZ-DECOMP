#include "../../include/types.h"

extern s32 g_main_8011EEC4_ResidentTableFlag;   /* nonzero once the requested file is already resident */
extern u8 g_main_8014B950_ArenaStart[];  /* overlay memory arena start (bump-pointer base) */
extern u8 g_main_80158878_ArenaEnd[];  /* overlay memory arena end (upper bound) */
extern s8 g_main_8010C60C_CurrentOverlayName[];  /* name of the currently loaded overlay, see f_main_8002B354_LoadOverlay */
extern u8 g_main_80142150_ScratchBuffer[];  /* generic scratch read buffer, shared across load modes */

extern s32 f_main_8002B3D8_GetResidentFile(s8 *name, void **outPtr, u8 mode); /* checks whether the file is already resident, returns its size */
extern void func_800F045C(void *pathBuf, const char *fmt, s8 *name); /* likely sprintf(pathBuf, fmt, name) */
extern s32 func_80026234(void *pathBuf, FileHandle **outHandle);  /* likely open(pathBuf) */
extern s32 f_main_800263C4_GetFileSize(FileHandle *handle, s32 *outSize);
extern s32 func_80026414(FileHandle *handle, void *buf, s32 size, void *outInfo); /* likely read(handle, buf, size) */
extern s32 f_main_8002662C_CloseFile(FileHandle **handlePtr);
extern void func_80010750(void *dst, void *src, u32 wordCount);    /* likely word copy */
extern void f_main_80025AB4_SetArenaPtr(void *ptr);
extern void func_800F1384(s32 flag);                               /* unknown, called unconditionally on entry */

/* Loads a file into the overlay memory arena, retrying up to 6 times.
   `mode` (low byte of arg2) selects the destination:
     0 or 4 - caller-supplied buffer (arg1)
     other  - shared scratch buffer (g_main_80142150_ScratchBuffer)
   mode 4 additionally just records the arena pointer instead of copying,
   and mode 1 bumps the arena pointer forward by the loaded size. */
u32 f_main_8002B4C4_LoadFile(s8 *name, void *arg1, s32 mode) {
    s8 path[56];
    FileHandle *h;
    s32 sz;
    s32 ri;
    void *buf;
    u32 r;
    s32 i;
    u8 m;

    func_800F1384(0);
    m = mode & 0xFF;

    if (g_main_8011EEC4_ResidentTableFlag != 0) {
        r = f_main_8002B3D8_GetResidentFile(name, (void **) arg1, 0);
        if (r != 0) {
            if (m == 4) {
                f_main_80025AB4_SetArenaPtr(g_main_80158878_ArenaEnd);
            }
            return r;
        }
    }

    for (i = 0; i < 6; i++) {
        func_800F045C(path, "\\BIN\\%s;1", name);

        if (func_80026234(path, &h) != 0) {
            continue;
        }

        f_main_800263C4_GetFileSize(h, &sz);
        m = mode & 0xFF;
        buf = (m == 0 || m == 4) ? arg1 : g_main_80142150_ScratchBuffer;

        if (func_80026414(h, buf, sz, &ri) != 0) {
            f_main_8002662C_CloseFile(&h);
            continue;
        }

        if (m == 4) {
            f_main_80025AB4_SetArenaPtr(g_main_80158878_ArenaEnd);
        } else if (m == 1) {
            s32 a = (sz + 3) & ~3;
            s32 left = g_main_80158878_ArenaEnd - g_main_8014B950_ArenaStart;

            if (left < a) {
                a = left;
            }
            sz = a;

            func_80010750(arg1, g_main_80142150_ScratchBuffer, (u32) a >> 2);
            f_main_80025AB4_SetArenaPtr(g_main_8014B950_ArenaStart + sz);
            g_main_8010C60C_CurrentOverlayName[0] = 0;
        }

        f_main_8002662C_CloseFile(&h);
        return (u32) sz;
    }

    return 0;
}
