#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */
extern ObjRecord **g_main_8011EEFC_ObjArray;  /* array of object pointers */
extern SubDefEntry D_8011F59C[];              /* table indexed by WeaponDef.unk00 */
extern void func_8002CED4(void *dst, s32 val); /* not yet decompiled */

/* If arg0's weapon holder object (unk30) is in mode 0xC and its own
   WeaponDef's SubDefEntry has an unk4 pointer set, mirrors the +0xC
   pointer-chain word from that holder's unk8 onto arg0's, offset by 0xC
   (see func_8002CED4 users for the same "mirror +0xC" pattern). */
void f_init_801619A0_SyncHolderIfSlotBusy(ObjRecord *arg0) {
    WeaponDef *def = &g_main_8011EEF8_WeaponDefs[arg0->weaponId];
    s32 objIdx = def->unk30;

    if (objIdx != -1) {
        ObjRecord *holder = g_main_8011EEFC_ObjArray[objIdx];

        if (holder->mode == 0xC) {
            WeaponDef *holderDef = &g_main_8011EEF8_WeaponDefs[holder->weaponId];
            SubDefEntry *sub = &D_8011F59C[holderDef->unk00];

            if (sub->unk4 != 0) {
                func_8002CED4((u8 *) arg0->unk8 + 0xC, *(s32 *) ((u8 *) holder->unk8 + 0xC));
            }
        }
    }
}
