#include "../../include/types.h"

/* Guessed: reaches through obj->unk1C into a sub-struct and rewrites its
   mode/flag word - OR in 0x08800000, then mask to 0xCFFCFFFF; if the
   OR'd value had bit 0x100 set, also OR 0x2000 into the +0x4 word. Always
   stores `mode` into the sub-struct's +0x15 byte. Looks like a GPU
   primitive tag/mode setup (bit shapes match primitive command flags),
   unconfirmed. */
void f_main_80087684_SetRenderFlags(u8 *obj, s8 mode) {
    u8 *sub = *(u8 **) (obj + 0x1C);
    s32 flags = *(s32 *) sub | 0x08800000;

    *(s32 *) sub = flags & 0xCFFCFFFF;
    if (flags & 0x100) {
        *(s32 *) (sub + 4) |= 0x2000;
    }
    sub[0x15] = mode;
}
