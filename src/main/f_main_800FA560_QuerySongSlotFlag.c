#include "../../include/types.h"

/* Not in asm/800.s: 0x800FA560 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c.

   "Query" counterpart to f_main_800FA404_DispatchSongSlot (same bounds
   checks and state grouping, but reads instead of acting). Together with
   g_main_8012F3CC_PlayFn (0x8012F3CC) and the two callbacks used by
   f_main_800FA404_DispatchSongSlot (0x8012F3D4/0x8012F3D8), this fn ptr
   (0x8012F3D0) is the 2nd of what looks like a 4-entry callback "vtable"
   at 0x8012F3CC..0x8012F3D8. */

extern s32 (*g_main_8012F3D0_unk)(SongSlot *slot);   /* fn ptr, state==4 case */

s32 f_main_800FA560_QuerySongSlotFlag(SongTree *tree, s32 idx) {
    SongSlot *slot;
    s32 state;
    u8 *raw;

    if (tree == 0) {
        return 0;
    }
    if (idx >= tree->count) {
        return 0;
    }
    if (idx < 0) {
        return 0;
    }

    slot = &tree->slots[idx];
    if (slot->handler == 0) {
        return 0;
    }

    state = slot->state;
    raw = (u8 *) slot;

    if (state == 4) {
        u8 *result;

        if (g_main_8012F3D0_unk == 0) {
            return 0;
        }
        result = (u8 *) (u64) g_main_8012F3D0_unk(slot);
        if (result == 0) {
            return 0;
        }
        return result[6] & 4;
    } else if (state < 5) {
        if (state <= 0) {
            return 0;
        }
        return *(s32 *) (raw + 0x1C);
    } else if (state == 5) {
        u8 *container = *(u8 **) (raw + 0x1C);
        s32 trackCount;
        s32 i;

        if (container == 0) {
            return 0;
        }
        trackCount = *(u8 *) (container + 7);
        if (trackCount == 0) {
            return 0;
        }

        for (i = 0; i < trackCount; i++) {
            u8 *track = *(u8 **) (container + 0x10);

            if (*(track + 6) & 4) {
                return 1;
            }
            container += 4;
        }
    }

    return 0;
}
