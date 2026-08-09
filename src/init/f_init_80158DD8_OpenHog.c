#include "../../include/types.h"

/* --- libc-style externs (not yet decompiled) --- */
extern void func_800F045C(void *pathBuf, const char *fmt, s8 *dir, s8 *name); /* likely sprintf(pathBuf, fmt, dir, name) */
extern s32 f_main_80026C8C_LoadResource(s8 *path, void *arg1, void **outPtr);

/* Builds a "\<dir>\<name>.HOG;1" path (format string at D_80158AB0) and
   hands it to f_main_80026C8C_LoadResource. Returns 1 if that call reports
   failure (result < 1), 0 on success. */
s32 f_init_80158DD8_OpenHog(s8 *dir, s8 *name, void *c, void *d) {
    s8 path[0x20];

    func_800F045C(path, "\\%s\\%s.HOG;1", dir, name);
    return f_main_80026C8C_LoadResource(path, d, (void **) c) < 1;
}
