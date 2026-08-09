#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */
extern void **g_main_8011EEFC_ObjArray;       /* array of object pointers */

extern void func_8002CED4(void *a0, s32 a1);

/* arg0: weapon-holder object with weaponId at +0x2, a pointer at +0x8 whose
   own +0x10 is a further pointer (unconfirmed chain, only touched here). */
void f_init_80161E28_UpdateWeaponTrack(void *arg0) {
    WeaponDef *def;
    void *obj;
    u8 mode;

    def = &g_main_8011EEF8_WeaponDefs[*(s16 *) ((u8 *) arg0 + 0x2)];
    mode = def->unk2A;

    if (def->unk30 != -1) {
        obj = g_main_8011EEFC_ObjArray[def->unk30];
        if (*((u8 *) obj + 0x26) == 7) {
            void *p = *(void **) ((u8 *) obj + 0x8);
            func_8002CED4((u8 *) *(void **) ((u8 *) arg0 + 0x8) + 0xC, *(s32 *) ((u8 *) p + 0xC));
            if (mode == 0x24) {
                void *inner = *(void **) (*(u8 **) ((u8 *) arg0 + 0x8) + 0x10);
                s32 *flags = (s32 *) ((u8 *) inner + 0x28);
                *flags |= 0x100000;
            }
        }
    }
}
