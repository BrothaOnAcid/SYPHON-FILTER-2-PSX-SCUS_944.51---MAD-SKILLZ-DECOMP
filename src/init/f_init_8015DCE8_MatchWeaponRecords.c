#include "../../include/types.h"

/* D_80127CC4 doubles as both the record count AND the base address of a
   large scratch region (part of the level-scratch block seeded by
   f_init_801591A0_ResetLevelState's D_80127CA0/D_8011F570); this function
   reaches well past that file's documented +0x78 bound, so offsets below
   are kept raw rather than forced into a named struct. Layout used here
   (all relative to &D_80127CC4):
     +0x00       count (s32, == D_80127CC4 itself)
     +0x2C       s16[count], record "modeA" key, indexed by record id
     +0x40       s16[count], record "modeB" key, indexed by record id
     +0xD4       s32[count], record id list
     +0xDC       s32[count], per-i match counter (parallel to +0xD4)
     +0xE4       s32[count], per-i running "flagged" sum
     +0xEC       s16[count][4], per-i match slots (weapon index), filled up
                 to 4 per i
     +0x100      void*[count], parallel pointer table (2nd phase) */
extern s32 D_80127CC4;
extern u8 D_80127D18[10];

extern WeaponDef *g_main_8011EEF8_WeaponDefs;
extern ObjRecord **g_main_8011EEFC_ObjArray;
extern s32 g_main_8011F564_ObjCount;
extern void *D_8011F570;

extern void func_800A9E1C(s32 a0, s32 a1, s32 a2, s32 a3, s32 arg4, s32 arg5); /* not yet decompiled */

/* First phase: for every live weapon-holder object, walks the +0xD4 record
   id list; a record matches when its modeA key equals the weapon's unk2A
   and its modeB key equals the weapon's unk4A (or is -1, wildcard). Each
   match records the weapon index into that record's +0xEC slot list (up to
   4) and bumps the record's +0xE4 sum when D_80127D18[recordId] is set (and
   the object's unk18->+0x8 s16 isn't positive) or when it's clear (and the
   object's flags0 bit 0x20 is set). If a record's match counter hits 4 the
   whole inner scan bails early for that weapon (matching the original
   "break to next weapon" control flow).
   Second phase: for every record whose modeA key isn't -1, and whose
   parallel +0x100 pointer isn't a "quoted" ('.' followed by a non-null
   byte) string, calls func_800A9E1C with fields pulled out of a much larger
   D_8011F570-based record (stride 0x100, offsets +0x100/0x108/0x124/0x12C/
   0x134) at index i. */
void f_init_8015DCE8_MatchWeaponRecords(void) {
    u8 *base = (u8 *) &D_80127CC4;
    s32 count = D_80127CC4;
    s32 i;

    if (count <= 0) {
        return;
    }

    if (g_main_8011F564_ObjCount > 0) {
        s32 j;

        for (j = 0; j < g_main_8011F564_ObjCount; j++) {
            ObjRecord *obj = g_main_8011EEFC_ObjArray[j];

            if (obj != 0) {
                u8 modeA = g_main_8011EEF8_WeaponDefs[j].unk2A;
                s16 modeB = g_main_8011EEF8_WeaponDefs[j].unk4A;

                for (i = 0; i < count; i++) {
                    s32 recordId = *(s32 *) (base + 0xD4 + i * 4);
                    s16 a = *(s16 *) (base + 0x2C + recordId * 2);
                    s16 b = *(s16 *) (base + 0x40 + recordId * 2);

                    if (a == modeA && (b == modeB || b == -1)) {
                        s32 *counter = (s32 *) (base + 0xDC + i * 4);

                        if (*counter >= 4) {
                            break;
                        }

                        *(s16 *) (base + 0xEC + i * 8 + *counter * 2) = (s16) j;
                        (*counter)++;

                        if (D_80127D18[recordId] == 0) {
                            if (obj->flags0 & 0x20) {
                                *(s32 *) (base + 0xE4 + i * 4) += 1;
                            }
                        } else if (*(s16 *) ((u8 *) obj->unk18 + 8) <= 0) {
                            *(s32 *) (base + 0xE4 + i * 4) += 1;
                        }
                    }
                }
            }
        }
    }

    for (i = 0; i < count; i++) {
        s32 recordId = *(s32 *) (base + 0xD4 + i * 4);
        s16 a = *(s16 *) (base + 0x2C + recordId * 2);

        if (a != -1) {
            u8 *ptr = *(u8 **) (base + 0x100 + i * 4);

            if (ptr[0] == '.' && ptr[1] != 0) {
                u8 *rec = (u8 *) D_8011F570 + i * 4;

                func_800A9E1C((s32) PTR_U32((u8 *) D_8011F570 + 0x14C + i * 0x100),
                              *(s32 *) (rec + 0x124), *(s32 *) (rec + 0x12C), *(s32 *) (rec + 0x134),
                              *(s32 *) (rec + 0x108), *(s32 *) (rec + 0x100));
            }
        }
    }
}
