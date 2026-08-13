#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */
extern ObjRecord **g_main_8011EEFC_ObjArray;  /* array of object pointers */
extern s32 g_main_8011F564_ObjCount;          /* count for g_main_8011EEFC_ObjArray */
extern SubDefEntry D_8011F59C[];              /* table indexed by WeaponDef.unk00, see f_init_8016112C_UpdateChainFlags */

extern void *func_80025AD0(s32 size);          /* not yet decompiled: bump allocator, see f_main_80025AD0_AllocDown */
extern u8 func_8002D4FC(s16 weaponId, WeaponDef *def); /* not yet decompiled */

/* Allocates g_main_8011F564_ObjCount ObjRecords (bump pool, stride 0x28) and
   an equal-length g_main_8011EEFC_ObjArray. For each weapon id: seeds the
   def's unk2A mode byte from D_8011F59C[def->unk00], then asks
   func_8002D4FC whether to actually instantiate an object for it. If not:
   fills in a fresh ObjRecord from the pool (weaponId, mode 6, unk18 pointed
   at def->unk38) and records it in both def->unk30 (index) and
   g_main_8011EEFC_ObjArray[id]; the pool cursor only advances on this path,
   so ids that are skipped don't consume a slot. If func_8002D4FC says skip,
   clears the def's own unk24/unk25/unk30 instead and leaves
   g_main_8011EEFC_ObjArray[id] NULL. */
void f_init_8015C648_InitWeaponObjects(void) {
    ObjRecord *pool = (ObjRecord *) func_80025AD0(g_main_8011F564_ObjCount * 0x28);
    s32 i;

    g_main_8011EEFC_ObjArray = (ObjRecord **) func_80025AD0(g_main_8011F564_ObjCount * 4);

    for (i = 0; i < g_main_8011F564_ObjCount; i++) {
        WeaponDef *def = &g_main_8011EEF8_WeaponDefs[i];

        g_main_8011EEFC_ObjArray[i] = 0;
        def->unk2A = (u8) D_8011F59C[def->unk00].unk0;

        if (!func_8002D4FC((s16) i, def)) {
            pool->weaponId = (s16) i;
            pool->flags0 = 0;
            pool->unk1 = 0;
            pool->flags27 = 0;
            pool->unk18 = &def->unk38;
            pool->unk8 = 0;
            pool->unkC = 0;
            pool->unk10 = 0;
            pool->unk14 = 0;
            pool->unk20 = 0;
            pool->unk4 = 0;
            pool->unk24 = 6;
            pool->unk25 = 0;
            pool->mode = 0;
            pool->unk1C = 0;

            def->unk30 = i;
            g_main_8011EEFC_ObjArray[i] = pool;
            pool++;
        } else {
            def->ammoType = 0;
            def->unk34 = 0;
        }
    }
}
