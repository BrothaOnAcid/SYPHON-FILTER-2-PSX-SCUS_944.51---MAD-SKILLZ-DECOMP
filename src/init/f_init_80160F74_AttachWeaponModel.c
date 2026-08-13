#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs;
extern ObjRecord **g_main_8011EEFC_ObjArray;

extern s8 *f_main_800F8258_strcpy(s8 *dst, const s8 *src);
extern s32 f_main_8002D4B4(const s8 *name, s32 *out); /* real out param is void**, but every caller here treats the
                                                          resolved value as an opaque 32-bit key, so it's declared as
                                                          s32* to stay ABI-safe on our 64-bit decomp host */
extern s32 f_init_80166C08_CreateWldRes(s32 key, s32 tag, s32 flags, WldRes **out);
extern void func_8002D340(s16 weaponId, void *out);            /* not yet decompiled */
extern void func_80025130(s32 id, s32 a1, void *in);            /* not yet decompiled */
extern void func_8002D2B4(s16 weaponId, void *out);              /* not yet decompiled */
extern void func_800249F4(s32 id, s32 a1, void *in);              /* not yet decompiled */
extern void func_80011084(s32 id);                                  /* not yet decompiled */

/* arg1: a name, resolved via f_main_8002D4B4 both as-is and as a copy of
   itself with the first character forced to 'X' (0x58), giving two ids
   (id0/id1). If id0
   resolved: clears the weapon's ammoType (NOTE: WeaponDef+0x24 conflict
   with the confirmed ammoType field, same as f_init_8015EE20's, left as a
   raw offset write) and creates a WldRes via f_init_80166C08_CreateWldRes
   (key=sp50, tag=&weaponDef+4, flags=0x04010000). On success, configures
   the fresh res (unk18=0, unk09=0x11, unk14=weaponId+0x2000) and runs it
   through func_8002D340/func_80025130 and func_8002D2B4/func_800249F4
   (using res->tag as the target id), then func_80011084(res->tag).

   Stores the (possibly NULL) result into the weapon-holder's ObjRecord
   (unk8); if a res was created AND sp54 resolved, also stashes sp54 into
   res->typeDef->unk24 and sets bit 8 on res->unk0A. Finally re-seeds the
   holder's unk18 to &WeaponDefs[holder->weaponId].unk38 (same pattern as
   f_init_8015C648_InitWeaponObjects). */
void f_init_80160F74_AttachWeaponModel(s16 arg0, s8 *arg1) {
    ObjRecord *holder = g_main_8011EEFC_ObjArray[arg0];
    s8 altName[64];
    s32 id0, id1;
    WldRes *res;

    f_main_800F8258_strcpy(altName, arg1);
    altName[0] = 0x58; /* 'X' */

    f_main_8002D4B4(arg1, &id0);
    f_main_8002D4B4(altName, &id1);

    if (id0 != 0) {
        WeaponDef *def = &g_main_8011EEF8_WeaponDefs[arg0];
        *(u32 *) ((u8 *) def + 0x24) = 0;

        f_init_80166C08_CreateWldRes(id0, (s32) (u64) ((u8 *) def + 4), 0x04010000, &res);

        if (res != 0) {
            u8 buf1[0x18];
            u8 buf2[0x18];
            s32 tag = res->tag;

            *(void **) ((u8 *) res + 0x18) = 0;
            res->unk09 = 0x11;
            res->unk14 = (s16) (arg0 + 0x2000);

            func_8002D340(arg0, buf1);
            func_80025130(tag, 0, buf1);

            func_8002D2B4(arg0, buf2);
            func_800249F4(tag, 0, buf2);

            func_80011084(tag);
        }
    } else {
        res = 0;
    }

    holder->unk8 = res;

    if (res != 0 && id1 != 0) {
        res->typeDef->unk24 = id1;
        *((u8 *) holder->unk8 + 0xA) |= 8;
    }

    holder->unk18 = (u8 *) &g_main_8011EEF8_WeaponDefs[holder->weaponId] + 0x38;
}
