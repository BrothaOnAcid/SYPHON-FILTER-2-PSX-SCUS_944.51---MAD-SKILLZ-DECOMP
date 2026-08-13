#include "../../include/types.h"

extern TypeDefCache g_main_80168AA8_TypeDefCache;
extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */
extern void **g_main_8011EEFC_ObjArray;       /* array of object pointers */
extern s32 g_main_8011F564_ObjCount;          /* count for g_main_8011EEFC_ObjArray */

extern void f_main_80025D3C_RemoveNode(ListHead *head, ListNode *node);
extern void func_80039D10(void *obj, s32 a1); /* not yet decompiled */
extern void func_80047D74(void *obj, s32 a1); /* not yet decompiled */

/* Two unrelated passes:
   1. Empties every live g_main_80168AA8_TypeDefCache slot (capped at 7),
      properly unlinking each node via f_main_80025D3C_RemoveNode instead of
      just dropping the list - the freed nodes go back to the shared
      free-list (g_main_8011EE38_FreeListHead) for reuse.
   2. Walks every object in g_main_8011EEFC_ObjArray; for ones in mode 7
      with flag bit 0x80 set (+0x1), calls func_80039D10, and - if the
      object's WeaponDef.unk4A has bit 4 set - also calls func_80047D74. */
void f_init_8015E850_ResetTypeDefCacheAndNotify(void) {
    s16 slotCount = g_main_80168AA8_TypeDefCache.count;
    s32 i;

    if (slotCount > 7) {
        slotCount = 7;
    }
    for (i = 0; i < slotCount; i++) {
        ListNode *node = g_main_80168AA8_TypeDefCache.values[i].first;

        while (node != 0) {
            ListNode *next = node->next;
            f_main_80025D3C_RemoveNode(&g_main_80168AA8_TypeDefCache.values[i], node);
            node = next;
        }
    }

    for (i = 0; i < g_main_8011F564_ObjCount; i++) {
        void *obj = g_main_8011EEFC_ObjArray[i];

        if (*((u8 *) obj + 0x26) == 7 && (*((u8 *) obj + 1) & 0x80)) {
            func_80039D10(obj, 0);
            if (g_main_8011EEF8_WeaponDefs[*(s16 *) ((u8 *) obj + 2)].unk4A & 4) {
                func_80047D74(obj, 0);
            }
        }
    }
}
