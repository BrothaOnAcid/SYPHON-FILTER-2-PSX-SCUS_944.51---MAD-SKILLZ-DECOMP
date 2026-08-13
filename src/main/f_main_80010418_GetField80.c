#include "../../include/types.h"

/* Returns the object's +0x80 field, or 0 if the handle is -1 (invalid). */
s32 f_main_80010418_GetField80(HandleObj80 *obj) {
    if (PTR_U32(obj) == 0xFFFFFFFF)
        return 0;
    return (s32) PTR_U32(obj->slots80);
}
