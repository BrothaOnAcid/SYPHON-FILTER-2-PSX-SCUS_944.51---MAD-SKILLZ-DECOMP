#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */
extern void **g_main_8011EEFC_ObjArray;       /* array of object pointers */
extern s32 g_main_8011F564_ObjCount;          /* count for g_main_8011EEFC_ObjArray */
extern void (*D_8011F558)(void *obj, u8 mode); /* not yet decompiled: optional callback */

/* guess: small exception table, { s32 count; s16 modes[..]; s16 unk4As[..]; }
   with the two parallel s16 arrays offset exactly 4 entries (8 bytes) apart
   - modes[] at +0x4, unk4As[] at +0xC. Exact capacities unconfirmed. */
extern s32 D_8012C758;

extern void func_801621D4(void *obj, u8 mode); /* not yet decompiled */

/* Walks every object in g_main_8011EEFC_ObjArray (stride 0x4C into
   g_main_8011EEF8_WeaponDefs runs in lock-step via `weaponDefOff`). For each
   live object: reads its WeaponDef's unk2A (mode) and unk4A (id), fires the
   optional D_8011F558 callback and func_801621D4 with (obj, mode), then
   scans D_8012C758's exception table for an entry whose mode matches and
   whose id either matches unk4A or is -1 (wildcard); on a hit, sets flag
   bit 0x40 on the object's +0x27 byte. */
void f_init_8015DF74_ApplyWeaponModeException(void) {
    s32 exceptionCount = D_8012C758;
    s32 i;
    s32 weaponDefOff = 0;

    for (i = 0; i < g_main_8011F564_ObjCount; i++, weaponDefOff += 0x4C) {
        void *obj = g_main_8011EEFC_ObjArray[i];

        if (obj != 0) {
            u8 *defBase = (u8 *) g_main_8011EEF8_WeaponDefs + weaponDefOff;
            u8 mode = *(defBase + 0x2A);
            s16 id = *(s16 *) (defBase + 0x4A);
            s32 j;

            if (D_8011F558 != 0) {
                D_8011F558(obj, mode);
            }
            func_801621D4(obj, mode);

            for (j = 0; j < exceptionCount; j++) {
                s16 excMode = *(s16 *) ((u8 *) &D_8012C758 + 0x4 + j * 2);
                s16 excId = *(s16 *) ((u8 *) &D_8012C758 + 0xC + j * 2);

                if (excMode == mode && (excId == id || excId == -1)) {
                    *((u8 *) obj + 0x27) |= 0x40;
                }
            }
        }
    }
}
