#include "../../include/types.h"

/* Same HogToc format as f_main_80026E00_FindResource (see its doc comment
   in types.h). Pairs with f_main_80026F4C_FindNamedIndex: given the index
   that returned, writes entry `index`'s size (bounds[index+1] -
   bounds[index], from the HogToc's boundary array) to *outSize. Returns 0
   on success, 1/0xE on bad args (NULL toc, negative or out-of-range
   index) - notably `index == -1` (not-found) hits the 0xE path and
   *outSize is left untouched. */
s32 f_main_80026D98_GetEntrySize(HogToc **toc, s32 index, s32 *outSize) {
    HogToc *t;

    if (toc == 0)
        return 1;

    t = *toc;
    if (t == 0)
        return 1;

    if (index < 0)
        return 0xE;
    if (index >= t->count)
        return 0xE;

    {
        s32 *bounds = (s32 *) ((u8 *) t + t->valuesOffset) + index;
        *outSize = bounds[1] - bounds[0];
    }

    return 0;
}
