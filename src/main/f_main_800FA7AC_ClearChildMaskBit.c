#include "../../include/types.h"

/* Not in asm/800.s: 0x800FA7AC falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c.

   Not reachable via any direct jal in the binary (splat's alabel scan
   confirms no boundary there) - it's a ChannelCb, stored into
   Rec48.unk44 as a raw address constant by
   f_main_800F9F68_PlaySongNode and invoked only indirectly through that
   function pointer (see f_main_800FCEA8_AllocateChannel /
   f_main_800FD020's firing sites). */

/* guess: on cmd 1-4, clears bit `channel` from the SongSlot's childMask
   (arg is the SongSlot* passed through as Rec48.unk40). */
void f_main_800FA7AC_ClearChildMaskBit(s32 channel, SongSlot *slot, s32 cmd) {
    if (cmd >= 1 && cmd < 5) {
        slot->childMask &= ~(1u << channel);
    }
}
