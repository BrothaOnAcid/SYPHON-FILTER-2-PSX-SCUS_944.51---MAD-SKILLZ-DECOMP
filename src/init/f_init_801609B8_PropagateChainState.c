#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs;
extern ObjRecord **g_main_8011EEFC_ObjArray;

extern void func_8002CED4(void *dst, s32 val); /* not yet decompiled */
extern void func_8002CFCC(void *obj, s32 val);  /* not yet decompiled */
extern void func_80017678(void *ctx, s32 val, s32 a2); /* not yet decompiled */

/* arg0: an ObjRecord. Follows arg0's weapon's chain slot (WeaponDef.unk30 -
   index into g_main_8011EEFC_ObjArray); no-op unless that slot is set AND
   the chained object's mode is 0xE.

   Stamps the chained weapon's own chainId = arg0's weaponId (matches the
   f_init_801604E0/f_init_8015E7AC chainId family). Then branches on the
   chained weapon's OWN chain slot:
     - unset (-1): syncs both objects' pointer-chains via func_8002CED4
       (same +0xC pattern as f_init_801604E0_SyncChainedWeapon), always
       falls through to the final step below.
     - set, and that next-next object's mode is 7: stamps arg0's own
       chainId = that object's weaponId, syncs via func_8002CFCC instead,
       and only falls through to the final step if that object's flags1
       bit 0x80 is NOT set (otherwise returns early).
     - set, mode != 7: no-op (matches the asm's shared fallthrough target
       at .L80160B88 for every other case).

   Final step: sets bit 2 on WeaponDefs[arg0->weaponId].unk24->unk4 - NOTE
   unk24 is read here as a POINTER, the third call site seen doing this
   (see f_init_8015EE20_AttachWeaponLink's note) against the single
   confirmed ammoType-u16 reading elsewhere; kept as a raw offset access
   rather than touching that field. */
void f_init_801609B8_PropagateChainState(ObjRecord *arg0) {
    s32 chainIdx = g_main_8011EEF8_WeaponDefs[arg0->weaponId].unk30;
    ObjRecord *chained;
    s32 nextIdx;
    void *ctx;

    if (chainIdx == -1) {
        return;
    }
    chained = g_main_8011EEFC_ObjArray[chainIdx];
    if (chained->mode != 0xE) {
        return;
    }

    nextIdx = g_main_8011EEF8_WeaponDefs[chainIdx].unk30;
    g_main_8011EEF8_WeaponDefs[chainIdx].chainId = arg0->weaponId;

    if (nextIdx == -1) {
        func_8002CED4((u8 *) chained->unk8 + 0xC, 0);
        func_8002CED4((u8 *) arg0->unk8 + 0xC, *(s32 *) ((u8 *) chained->unk8 + 0xC));
    } else {
        ObjRecord *next = g_main_8011EEFC_ObjArray[nextIdx];
        if (next->mode != 7) {
            return;
        }
        g_main_8011EEF8_WeaponDefs[arg0->weaponId].chainId = next->weaponId;
        func_8002CFCC(chained, *(s32 *) ((u8 *) next->unk8 + 0xC));
        func_8002CFCC(arg0, *(s32 *) ((u8 *) next->unk8 + 0xC));
        ctx = *(void **) ((u8 *) &g_main_8011EEF8_WeaponDefs[arg0->weaponId] + 0x24);
        func_80017678(ctx, *(s32 *) ((u8 *) arg0->unk8 + 0xC), 0);
        if (next->unk1 & 0x80) {
            return;
        }
        *(u32 *) ((u8 *) ctx + 4) |= 2;
        return;
    }

    ctx = *(void **) ((u8 *) &g_main_8011EEF8_WeaponDefs[arg0->weaponId] + 0x24);
    func_80017678(ctx, *(s32 *) ((u8 *) arg0->unk8 + 0xC), 0);
    *(u32 *) ((u8 *) ctx + 4) |= 2;
}
