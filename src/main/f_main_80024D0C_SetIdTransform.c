#include "../../include/types.h"

/* --- libc-style externs (not yet decompiled) --- */
extern void func_80024D48(s32 id, s32 parentXform, MATRIX *mtx);

/* Wrapper called by f_init_80166728_LinkNpcSpawnRecords right after
   resolving an id: looks up id's +0x20 field (parent-ish pointer) and, if
   present, that parent's own +0x20 field, then forwards to func_80024D48
   along with the caller's matrix. */
void f_main_80024D0C_SetIdTransform(s32 id, MATRIX *mtx) {
    s32 parent = *(s32 *) ((u8 *) (u64) id + 0x20);
    s32 parentXform = parent ? *(s32 *) ((u8 *) (u64) parent + 0x20) : 0;

    func_80024D48(id, parentXform, mtx);
}
