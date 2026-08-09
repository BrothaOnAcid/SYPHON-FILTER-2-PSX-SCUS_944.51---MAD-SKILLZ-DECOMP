#include "../../include/types.h"

/* Guessed object with a lookup id at +0x02. */
typedef struct {
    u8 _pad00[0x2];
    s16 id;   /* +0x02 */
} IdObj;

/* --- libc-style externs (not yet decompiled) --- */
extern void func_8002D3A8(s16 id, s32 *outValue, s32 *outFlag);

/* Looks up obj->id via func_8002D3A8; returns true only if the lookup
   reports success (outFlag != 0) and the returned value is non-zero. */
s32 f_init_8015E6E8_CheckEntry(IdObj *obj) {
    s32 value;
    s32 flag = 0;

    func_8002D3A8(obj->id, &value, &flag);

    if (flag == 0)
        return 0;
    return value != 0;
}
