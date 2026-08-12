#include "../../include/types.h"

/* Classic PSYQ libgpu addPrim(): links `prim` into the ordering-table chain
   right after `ot`. Both point at a GPU packet's tag word, whose low 24 bits
   hold the (masked) address of the next packet and whose top byte holds the
   packet's own length/type tag - the tag byte is preserved on both words,
   only the address field is rewritten. */
void f_main_80013000_AddPrim(void *ot, void *prim) {
    u32 *p = (u32 *) prim;
    u32 *o = (u32 *) ot;

    *p = (*p & 0xFF000000) | (*o & 0xFFFFFF);
    *o = (*o & 0xFF000000) | (PTR_U32(p) & 0xFFFFFF);
}
