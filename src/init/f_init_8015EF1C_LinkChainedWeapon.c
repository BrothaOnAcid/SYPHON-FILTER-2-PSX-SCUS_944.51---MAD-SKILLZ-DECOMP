#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs; /* weapon definitions, indexed by weapon id */
extern ObjRecord **g_main_8011EEFC_ObjArray;  /* array of object pointers */

extern void func_8002CED4(void *a0, s32 a1);    /* not yet decompiled */
extern void func_8015EE20(ObjRecord *a0, ObjRecord *a1); /* not yet decompiled */
extern s32 func_80045D88(s32 chainVal, u8 arg1, s32 *outA, s32 *outB, s32 arg4); /* not yet decompiled */
extern ObjRecord *func_80045AA4(s16 id, s16 arg1); /* guess: look up/create a track object by id */

/* Another sibling of the chain family (f_init_8015E7AC_ResolveChainedWeapon,
   f_init_801604E0_SyncChainedWeapon, f_init_8015F314_ResolveObjChain). Sets
   the linked weapon's (via arg0's own weapon's `unk30` objSlot) chainId
   back to arg0's own weapon id, then:
     - if the linked object's mode is 0xE and arg0's weapon has no chainId
       of its own yet: follows one more `unk30` hop off the linked object's
       own weapon, and if that hop's object is in mode 7, mirrors a data
       word via func_8002CED4, conditionally clones some state via
       func_8015EE20 and a raw WeaponDef+0x24 word copy, then (if
       func_80045D88 resolves a further target) cross-links three weapons'
       chainId fields together and, if the mode is still unchanged and a
       flag bit is set, mirrors more state onto the newly resolved object.
     - if the linked object's mode is 0xB: just mirrors a data word the
       other way via func_8002CED4.
   Heavily unconfirmed past the first chainId write. */
void f_init_8015EF1C_LinkChainedWeapon(ObjRecord *arg0) {
    s16 weaponId = arg0->weaponId;
    s32 linkIdx = g_main_8011EEF8_WeaponDefs[weaponId].unk30;
    ObjRecord *linked;
    u8 mode;

    if (linkIdx == -1)
        return;

    linked = g_main_8011EEFC_ObjArray[linkIdx];
    g_main_8011EEF8_WeaponDefs[linkIdx].chainId = weaponId;

    mode = linked->mode;
    if (mode == 0xE) {
        s16 ownChainId = g_main_8011EEF8_WeaponDefs[weaponId].chainId;
        s32 hopIdx;
        ObjRecord *hopObj;
        u8 hopMode;

        if (ownChainId != -1)
            return;

        hopIdx = g_main_8011EEF8_WeaponDefs[linked->weaponId].unk30;
        if (hopIdx == ownChainId) /* == -1 */
            return;

        hopObj = g_main_8011EEFC_ObjArray[hopIdx];
        hopMode = hopObj->mode;
        if (hopMode != 7)
            return;

        func_8002CED4((u8 *) arg0->unk8 + 0xC, *(s32 *) ((u8 *) hopObj->unk8 + 0xC));

        if (hopObj->unk1 & 0x80) {
            func_8015EE20(arg0, hopObj);
            /* raw 32-bit copy at WeaponDef+0x24 - straddles the documented
               ammoType(u16)/unk28(s16) fields, so treated as an opaque word
               rather than named accessors */
            *(s32 *) ((u8 *) &g_main_8011EEF8_WeaponDefs[hopObj->weaponId] + 0x24) =
                *(s32 *) ((u8 *) &g_main_8011EEF8_WeaponDefs[weaponId] + 0x24);
        }

        {
            s32 sp18 = linked->weaponId;
            s32 sp1C;

            if (func_80045D88(hopIdx, g_main_8011EEF8_WeaponDefs[linked->weaponId].unk2A, &sp18, &sp1C, 0) & 0xFF) {
                ObjRecord *found = g_main_8011EEFC_ObjArray[sp1C];
                ObjRecord *newObj = func_80045AA4(found->weaponId, 9);
                s16 newWeaponId = newObj->weaponId;

                if (newWeaponId != found->weaponId) {
                    g_main_8011EEF8_WeaponDefs[weaponId].chainId = newWeaponId;
                    g_main_8011EEF8_WeaponDefs[newWeaponId].chainId = (u16) weaponId;
                    g_main_8011EEF8_WeaponDefs[found->weaponId].chainId = (u16) newWeaponId;

                    if (hopObj->mode == hopMode) {
                        func_8002CED4((u8 *) newObj->unk8 + 0xC, *(s32 *) ((u8 *) hopObj->unk8 + 0xC));

                        if (hopObj->unk1 & 0x80) {
                            *(s32 *) ((u8 *) &g_main_8011EEF8_WeaponDefs[newWeaponId] + 0x24) =
                                *(s32 *) ((u8 *) &g_main_8011EEF8_WeaponDefs[weaponId] + 0x24);
                            newObj->flags0 |= 2;
                        }
                    }
                }
            }
        }
    } else if (mode == 0xB) {
        func_8002CED4((u8 *) linked->unk8 + 0xC, *(s32 *) ((u8 *) arg0->unk8 + 0xC));
    }
}
