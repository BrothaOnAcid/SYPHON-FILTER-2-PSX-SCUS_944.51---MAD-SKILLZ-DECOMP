#include "../../include/types.h"

/* Reads 2 raw bytes from cur->pos into *out (byte-by-byte, matching the
   asm's per-byte store/increment), advancing cur->pos by 2. Callers pass
   the address of an s16 (or reuse the cursor itself as the output), so
   this effectively copies a little-endian 16-bit value out of the
   token stream. */
void f_init_801594F4_ReadTokenWord(TokenCursor *cur, s16 *out) {
    u8 *dst = (u8 *) out;
    u8 *end = dst + 2;

    while (dst < end) {
        *dst = *cur->pos;
        cur->pos++;
        dst++;
    }
}
