#include "../../include/types.h"

extern WeaponDef *g_main_8011EEF8_WeaponDefs;  /* weapon definitions, indexed by weapon id */
extern ObjRecord **g_main_8011EEFC_ObjArray;   /* array of object pointers */
extern s32 g_main_8011F564_ObjCount;           /* count for g_main_8011EEFC_ObjArray */
extern SubDefEntry g_main_8011F59C_SubDefs[];  /* table indexed by WeaponDef.unk00 */
extern u8 *g_main_8011F5B8_Records;            /* stride-0x10 records, count g_main_8011F664_RecordCount */
extern s32 g_main_8011F664_RecordCount;
extern s32 D_8011F60C;                         /* not yet decompiled: default-duration lookup context */

extern s32 func_800F9244(u8 *str, void *ctx);          /* not yet decompiled */
extern s32 func_800F8228(u8 *str, const char *suffix);  /* not yet decompiled */
extern s32 func_80026E00(s32 ctx, u8 *str, s32 *out);    /* not yet decompiled */
extern void f_init_80166A70_CreateWldResEx(s32 key, s32 arg1, s32 arg2, u8 flag, WldResEx **out, s32 weight);
extern void func_80166C08(s32 key, void *arg1, u32 flags, WldResEx **out); /* not yet decompiled */
extern void func_800A3A00(void *handle, WldResEx *req);       /* not yet decompiled */
extern void func_8002D340(s16 weaponId, void *out);            /* not yet decompiled */
extern void func_80025130(s32 ctx, s32 arg1, void *in);         /* not yet decompiled */
extern void func_8002D2B4(s16 weaponId, void *out);             /* not yet decompiled */
extern void func_800249F4(s32 ctx, s32 arg1, void *in);         /* not yet decompiled */
extern void func_80011084(s32 ctx);                             /* not yet decompiled */

/* Builds (or clears) a weapon's WldResEx request from its SubDefEntry:
   - unk4[0]==0 clears the object's request slot (unk8=0) and bails.
   - unk8==0 (no intern key) also clears the request, skipping setup entirely.
   - Otherwise the byte 3 before wherever func_800F9244 lands in the unk4
     string picks the mode: 'H' builds a "count occurrences of this
     sub-def id across live weapons/records" weighted request (or, when
     arg1==0, truncates the filename and appends ".HAN" to look up a
     default duration instead); 'E' and the fallback both hand off to
     func_80166C08 with different flags/key args.
   - Any resulting request gets its clip (arg0+0x2000) and context wired up
     via func_8002D340/func_80025130/func_8002D2B4/func_800249F4/func_80011084,
     then subDef->flags2 bits tweak WldResEx.unkA/unkB and the linked
     WldTypeDef's flags28.
   - Finally obj->unk8 is (re)set to the request and, if subDef->unk10 is
     set, stashed into the WldTypeDef too (unk24) with WldResEx.unkA |= 8. */
void f_init_8015BB50_DispatchWeaponSound(s16 arg0, s32 arg1) {
    s32 subDefId = g_main_8011EEF8_WeaponDefs[arg0].unk00;
    SubDefEntry *subDef = &g_main_8011F59C_SubDefs[subDefId];
    ObjRecord *obj = g_main_8011EEFC_ObjArray[arg0];
    WldResEx *req;
    u16 flags2;

    if (*subDef->unk4 == 0) {
        obj->unk8 = 0;
        return;
    }

    if (subDef->unk8 != 0) {
        s32 off = func_800F9244(subDef->unk4, &g_main_8011EEFC_ObjArray[arg0]);
        u8 marker = *(subDef->unk4 + off - 3);

        if (marker == 'H') {
            if (arg1 == 0) {
                s32 dur;
                s32 off2 = func_800F9244(subDef->unk4, &g_main_8011EEFC_ObjArray[arg0]);

                *(subDef->unk4 + off2 - 4) = 0;
                func_800F8228(subDef->unk4, ".HAN");
                func_80026E00(D_8011F60C, subDef->unk4, &dur);
                f_init_80166A70_CreateWldResEx(subDef->unk8, dur, 0, 0, &req, 0);
                req->base.flags8 |= 0x20;
            } else {
                s32 weight = 0;
                s32 i;

                for (i = 0; i < g_main_8011F564_ObjCount; i++) {
                    if (g_main_8011EEF8_WeaponDefs[i].unk00 == subDefId) {
                        weight += 1;
                    }
                }

                for (i = 0; i < g_main_8011F664_RecordCount; i++) {
                    u8 *rec = g_main_8011F5B8_Records + i * 0x10;
                    u8 count = rec[5];
                    u8 *entries = *(u8 **) (rec + 0xC);
                    u8 *entriesEnd = entries + count;

                    while (entries < entriesEnd) {
                        if (*entries == subDefId) {
                            weight += 5;
                        }
                        entries++;
                    }
                }

                if (weight >= 2) {
                    weight = (weight < 5) ? weight : 5;
                } else {
                    weight = 1;
                }

                f_init_80166A70_CreateWldResEx(subDef->unk8, arg1, 0, 1, &req, weight);
            }
        } else if (marker == 'E') {
            func_80166C08(subDef->unk8, (u8 *) g_main_8011EEF8_WeaponDefs + arg0 * 0x4C + 0x4, 0x01100000, &req);
            if (subDef->unk10 != 0) {
                func_800A3A00(subDef->unk10, req);
            }
            func_800A3A00((void *) (u64) subDef->unk8, req);
        } else {
            u8 *def = (u8 *) g_main_8011EEF8_WeaponDefs + arg0 * 0x4C;

            *(u32 *) (def + 0x24) = 0; /* zeroes ammoType (+0x24) and the two bytes after it */
            func_80166C08(subDef->unk8, def + 0x4, 0x04000000, &req);
        }

        if (req != 0) {
            s32 ctx = req->base.tag;

            req->base.unk14 = (u32) (u16) (arg0 + 0x2000);
            {
                u8 spA[0x20]; /* opaque scratch buffer for func_8002D340/func_80025130 */
                func_8002D340(arg0, spA);
                func_80025130(ctx, 0, spA);
            }
            {
                u8 spB[0x20]; /* opaque scratch buffer for func_8002D2B4/func_800249F4 */
                func_8002D2B4(arg0, spB);
                func_800249F4(ctx, 0, spB);
            }
            func_80011084(ctx);

            flags2 = g_main_8011F59C_SubDefs[subDefId].flags2;
            if (flags2 & 1) {
                req->base.unk0A |= 0x20;
                req->base.typeDef->flags28 |= 0x8000;
            } else if (flags2 & 2) {
                req->base.flagsB |= 1;
            }

            if (g_main_8011F59C_SubDefs[subDefId].flags2 & 4) {
                req->base.flagsB |= 8;
            }
        }
    } else {
        req = 0;
    }

    obj->unk8 = req;
    if (req != 0 && subDef->unk10 != 0) {
        req->base.typeDef->unk24 = (u32) (u64) subDef->unk10;
        req->base.unk0A |= 8;
    }

    obj->unk18 = (u8 *) g_main_8011EEF8_WeaponDefs + obj->weaponId * 0x4C + 0x38;
}
