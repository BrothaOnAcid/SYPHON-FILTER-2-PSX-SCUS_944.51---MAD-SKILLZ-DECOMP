#include "../../include/types.h"

extern s32 f_main_80026E00_FindResource(HogArchive *archive, const s8 *name, void **out);

/* Thin stack-frame wrapper, args passed through unchanged. Called with
   "MODEL.HMD" as `name` from func_80163D40 (not yet decompiled - large,
   see DOC.md task list) to resolve the model's resource data pointer out
   of an already-loaded HOG-style archive. */
s32 f_init_80163208_FindResource(HogArchive *archive, const s8 *name, void **out) {
    return f_main_80026E00_FindResource(archive, name, out);
}
