#include "../../include/types.h"

extern s32 f_main_800F8248_strcmp(const s8 *a, const s8 *b);

/* Looks up `name` in `archive`'s TOC (see HogToc in types.h) and writes the
   resolved resource data pointer to *out. Returns 0 on success, 1 on bad
   args (NULL archive/toc/name/out), 5 if the name isn't found. */
s32 f_main_80026E00_FindResource(HogArchive *archive, const s8 *name, void **out) {
    HogToc *toc;
    s8 *names;
    s32 i;
    s32 off;

    if (archive == 0 || archive->toc == 0 || name == 0 || out == 0) {
        return 1;
    }

    toc = archive->toc;
    if (toc->count <= 0) {
        return 5;
    }

    off = 0;
    for (i = 0; i < toc->count; i++) {
        names = (s8 *) toc + toc->namesOffset;

        if (f_main_800F8248_strcmp(name, names + off) == 0) {
            break;
        }

        while (names[off] != 0) {
            off++;
        }
        off++;
    }

    if (i >= toc->count) {
        return 5;
    }

    {
        s32 *values = (s32 *) ((s8 *) toc + toc->valuesOffset);
        *out = (s8 *) toc + toc->dataOffset + values[i];
    }

    return 0;
}
