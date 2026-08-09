#include "../../include/types.h"

/* Returns 1 on bad args (NULL handle or NULL outSize). Otherwise writes the
   file size to *outSize - rounded up to a 2048-byte CD sector unless
   handle->mode is negative - and returns 0. */
s32 f_main_800263C4_GetFileSize(FileHandle *handle, s32 *outSize) {
    if (handle == 0 || outSize == 0) {
        return 1;
    }

    if (handle->mode < 0) {
        *outSize = handle->size;
    } else {
        *outSize = (handle->size + 0x7FF) & ~0x7FF;
    }

    return 0;
}
