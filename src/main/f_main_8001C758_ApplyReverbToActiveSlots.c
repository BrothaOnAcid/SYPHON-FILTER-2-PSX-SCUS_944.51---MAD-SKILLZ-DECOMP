#include "../../include/types.h"

/* --- not yet decompiled --- */
extern void func_800205E4(SoundSlot *slot, s32 arg1, s32 arg2, void *dst);

extern SoundSlot g_main_80121F40_SoundSlots[0x3C];
extern u16 g_main_8011EC72_ReverbMode;
extern u16 g_main_80120906_ReverbTable[];
extern SoundParamSlot g_main_80120514_SoundParams[];

/* Walks the 60-entry g_main_80121F40_SoundSlots table and, for every active
   slot, pushes the current reverb-table entry (selected by
   g_main_8011EC72_ReverbMode) and the `category`'s param value into
   func_800205E4. Called by f_main_8001C828_ProcessSoundSources right after
   switching a source's tag string, to re-apply reverb to already-playing
   slots. */
void f_main_8001C758_ApplyReverbToActiveSlots(s32 category) {
    SoundSlot *slot = g_main_80121F40_SoundSlots;
    s32 i;

    for (i = 0; i < 0x3C; i++, slot++) {
        if (slot->active != 0) {
            func_800205E4(slot, g_main_80120906_ReverbTable[g_main_8011EC72_ReverbMode * 0xF0] & 0x10,
                          g_main_80120514_SoundParams[category].val, slot->unk28);
        }
    }
}
