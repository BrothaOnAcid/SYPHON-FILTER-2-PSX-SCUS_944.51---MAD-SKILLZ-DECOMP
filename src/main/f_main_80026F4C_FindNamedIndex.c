#include "../../include/types.h"

extern s32 f_main_800F8248_strcmp(const s8 *a, const s8 *b);

/* Same HogToc format as f_main_80026E00_FindResource (see its doc comment
   in types.h), but returns the matching entry's INDEX instead of
   resolving its data pointer - paired with f_main_80026D98_GetEntrySize
   by f_init_80168190_LoadSongTree to size a "<name>.VB" resource before
   streaming it. `toc` is passed as a pointer-to-pointer (dereferenced
   once) matching how f_init_80168298_InitSoundSystem stores its resolved
   archive pointer. Returns -1 if `name` isn't found. */
s32 f_main_80026F4C_FindNamedIndex(HogToc **toc, const s8 *name) {
    HogToc *t = *toc;
    s8 *names = (s8 *) t + t->namesOffset;
    s32 off = 0;
    s32 i;

    for (i = 0; i < t->count; i++) {
        if (f_main_800F8248_strcmp(name, names + off) == 0)
            return i;

        while (names[off] != 0)
            off++;
        off++;
    }

    return -1;
}
