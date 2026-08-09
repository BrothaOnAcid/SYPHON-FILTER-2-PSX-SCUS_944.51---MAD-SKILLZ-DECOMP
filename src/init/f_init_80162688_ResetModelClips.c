#include "../../include/types.h"

extern s32 g_main_8011F660_ModelCount;
extern WldModelHandle *g_main_8011F680_ModelHandles;
extern WldClipSlot *g_main_8011F968_ClipSlots;
extern s32 g_main_8011F96C_ClipSlotCount;
extern u8 g_main_80120900_ModelFlagsBase[]; /* per-model flag bytes at +0x13D, count = ModelCount */

extern void func_800A21B4(s32 modelIndex); /* guess: kick off a (re)load for this model */
extern void func_800A3278(void);

/* Clears the per-model "in use" flags and re-resolves the streaming data
   pointer for any clip slot whose model finished loading. */
void f_init_80162688_ResetModelClips(void) {
    s32 i;
    WldClipSlot *c;
    WldRes *res;
    WldTypeDef *typeDef;

    for (i = 0; i < g_main_8011F660_ModelCount; i++)
        g_main_80120900_ModelFlagsBase[0x13D + i] = 0;

    for (i = 0; i < g_main_8011F96C_ClipSlotCount; i++) {
        c = &g_main_8011F968_ClipSlots[i];
        res = (WldRes *) (u64) (u32) c->unk04;
        typeDef = res->typeDef;

        if (c->free == 0) {
            if (typeDef->dataPtr == 0)
                func_800A21B4(c->modelIndex);
        } else if (c->free >= 2) {
            if (typeDef->dataPtr == 0)
                typeDef->dataPtr = (s32) (PTR_U32(g_main_8011F680_ModelHandles[c->modelIndex].subRes[0]) & 0x7FFFFFFF);
        }
    }

    func_800A3278();
}
