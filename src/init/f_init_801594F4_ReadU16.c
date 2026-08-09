#include "../../include/types.h"

/* Reads 2 bytes from the stream's cursor into `out`, advancing the
   cursor one byte at a time. Sibling of func_80159530 (same shape, 4
   bytes). */
void f_init_801594F4_ReadU16(ByteStream *stream, u8 *out) {
    u8 *end = out + 2;

    do {
        *out = *stream->cursor;
        stream->cursor++;
        out++;
    } while (out < end);
}
