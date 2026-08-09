#include "../../include/types.h"

/* Not in asm/800.s: 0x800FA404 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c.

   Sibling of f_main_800F9F68_PlaySongNode (per knowledge.txt "same
   top-of-function shape") - shares its bounds checks (tree->count,
   idx, slot->handler) but diverges after that. Its state values overlap
   SongSlot.state (0-5+) but the +0x1C field is read here as a raw pointer
   rather than the bitmask PlaySongNode treats it as, so it's NOT modeled
   via SongSlot for that field - kept as raw offset arithmetic. Called from
   f_main_800F9F68_PlaySongNode's state==2 case as func_800FA404(tree,
   idx). */

extern s32 (*g_main_8012F3D4_unk)(void *slot);        /* fn ptr, state==4 case */
extern s32 (*g_main_8012F3D8_unk)(void *track);        /* fn ptr, state==5 case */

extern void f_main_801050B4_CancelScheduledSlot(void *tree, s32 idx);
extern s32 f_main_800FCE28_SetStopCallback(void (*cb)(void));
extern void f_main_800FD4C0_NotifyAndClearMask(u32 mask);
extern void f_main_800FCE58_FlushStopCallback(void);

void f_main_800FA404_DispatchSongSlot(SongTree *tree, s32 idx) {
    SongSlot *slot;
    s32 state;
    u8 *raw;

    if (tree == 0) {
        return;
    }
    if (idx >= tree->count) {
        return;
    }
    if (idx < 0) {
        return;
    }

    slot = &tree->slots[idx];
    if (slot->handler == 0) {
        return;
    }

    f_main_801050B4_CancelScheduledSlot(tree, idx);

    state = slot->state;
    raw = (u8 *) slot;

    if (state == 4) {
        if (g_main_8012F3D4_unk != 0) {
            g_main_8012F3D4_unk(slot);
        }
    } else if (state < 5) {
        u32 mask;

        if (state <= 0) {
            return;
        }
        mask = *(u32 *) (raw + 0x1C);
        if (mask == 0) {
            return;
        }
        f_main_800FCE28_SetStopCallback(0);
        f_main_800FD4C0_NotifyAndClearMask(mask);
        f_main_800FCE58_FlushStopCallback();
    } else if (state == 5) {
        u8 *container;
        s32 trackCount;
        s32 i;

        if (g_main_8012F3D8_unk == 0) {
            return;
        }
        container = *(u8 **) (raw + 0x1C);
        if (container == 0) {
            return;
        }
        trackCount = *(u8 *) (container + 7);
        if (trackCount == 0) {
            return;
        }

        for (i = 0; i < trackCount; i++) {
            void *track = *(void **) (container + 0x10);
            g_main_8012F3D8_unk(track);
            container += 4;
        }
    }
}
