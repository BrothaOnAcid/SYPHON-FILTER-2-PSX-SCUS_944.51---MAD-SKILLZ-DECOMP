#include "../../include/types.h"

extern HogArchive *D_8011F60C;

extern s32 f_main_80026E00_FindResource(HogArchive *archive, const s8 *name, void **out);

/* Thin wrapper over f_main_80026E00_FindResource using the fixed D_8011F60C
   archive: on failure (nonzero result) *out is forced to NULL instead of
   being left untouched. Returns f_main_80026E00_FindResource's result. */
s32 f_main_8002D4B4(const s8 *name, void **out) {
    s32 result = f_main_80026E00_FindResource(D_8011F60C, name, out);

    if (result != 0) {
        *out = 0;
    }

    return result;
}
