#include "../../include/types.h"

/* Guessed level-data header, resolved from a loaded blob (base = `self`
   below). Every field except unk30 is a BYTE OFFSET from `self` that gets
   turned into an absolute pointer by adding `self` to it - a classic
   relocation-on-load format. Confirmed by this function
   (f_init_80158E3C_LoadLevelHeader), the only place any of these fields
   are read. */
typedef struct {
    u8 _pad00[0x4];
    s32 groupCount;   /* +0x04: entry count for groupA/groupB below */
    s32 objCount;     /* +0x08: entry count, see g_main_8011F564_ObjCount */
    s32 subDefCount;  /* +0x0C: entry count, see g_main_8011F59C_SubDefs */
    s32 recordCount;  /* +0x10: entry count for the stride-0x10 records */
    s32 bitCount;     /* +0x14: see g_main_8011F24C_BitCount */
    s32 unk18;        /* +0x18: copied verbatim into g_main_8011F568_unk */
    s32 subDefsOff;   /* +0x1C: -> g_main_8011F59C_SubDefs */
    s32 weaponDefsOff; /* +0x20: -> g_main_8011EEF8_WeaponDefs */
    s32 groupAOff;     /* +0x24: -> g_main_8011F590_GroupA */
    s32 recordsOff;     /* +0x28: -> g_main_8011F5B8_Records, only if recordCount > 0 */
    s32 groupBOff;       /* +0x2C: -> g_main_8011F58C_GroupB */
    u8 tail[1];            /* +0x30: flexible trailing data, see g_main_8011F254_HeaderTail */
} LevelHeader;

/* Guessed streaming-context pointer, only used here to derive the header's
   base address in the "already loaded" path. */
typedef struct {
    u8 _pad00[0xC];
    s32 unkC;
    u8 _pad10[0x30 - 0x10];
    u8 *unk30;
} LevelStreamCtx;

extern LevelStreamCtx *D_8011F598;
extern LevelHeader *g_main_8011F5A4_LevelHeader;
extern s32 g_main_8011F560_SubDefCount;
extern s32 g_main_8011F564_ObjCount;
extern s32 g_main_8011F568_unk;
extern s32 g_main_8011F664_RecordCount;
extern s32 g_main_8011F24C_BitCount;
extern void *g_main_8011F254_HeaderTail;
extern void *g_main_8011F58C_GroupB;
extern void *g_main_8011F590_GroupA;
extern SubDefEntry *g_main_8011F59C_SubDefs;
extern s32 g_main_8011F5A8_GroupCount;
extern u8 *g_main_8011F5B8_Records;
extern WeaponDef *g_main_8011EEF8_WeaponDefs;
extern s16 D_8012B02C;
extern s32 D_8010C874[];
extern u8 D_80158AC0[]; /* not yet decompiled: string literal */

extern void func_800F045C(void *dst, u8 *fmt, s32 a, s32 b); /* not yet decompiled */
extern u32 f_main_8002B0D0_LoadFileAlloc(s8 *name, void **outPtr, s32 size);
extern void func_80080E80(s32 index);                           /* not yet decompiled */

/* If `alreadyLoaded` (low byte of arg0): reuses D_8011F598's own
   base+offset (unk30+unkC) as the level blob. Otherwise formats/resolves a
   file via func_800F045C + f_main_8002B0D0_LoadFileAlloc into
   g_main_8011F5A4_LevelHeader and uses that pointer itself as the blob
   base.

   Either way, resolves every LevelHeader field into its absolute-pointer
   global (see LevelHeader's own field comments), then walks and relocates
   (self-relative -> absolute) four separate embedded tables:
     - g_main_8011F59C_SubDefs[0..subDefCount): unk4/unkC pointer fields
     - g_main_8011EEF8_WeaponDefs[0..objCount): unk2C pointer field (only
       if nonzero)
     - g_main_8011F5B8_Records[0..recordCount) (stride 0x10): unk8/unkC
       pointer fields, plus - if unk4 (a per-record count) > 0 - a
       further indirect s32[] pointed to by unk8, each entry relocated in
       turn. func_80080E80(i) is called once per record before the fixups.
     - groupA/groupB (parallel stride-8 arrays, groupCount entries): each
       side's unk4 pointer field is relocated, then groupA's own unk0-sized
       s32[] (via the just-relocated unk4) has each entry turned from a
       weapon id into a WeaponDef* (g_main_8011EEF8_WeaponDefs + id*0x4C). */
void f_init_80158E3C_LoadLevelHeader(s32 arg0) {
    u8 *self;
    LevelHeader *hdr;
    s32 i, j;

    if (arg0 & 0xFF) {
        self = D_8011F598->unk30 + D_8011F598->unkC;
        g_main_8011F5A4_LevelHeader = (LevelHeader *) self;
    } else {
        u8 sp10[0x50];
        s32 key = D_8010C874[D_8012B02C];

        func_800F045C(sp10, D_80158AC0, key, key);
        f_main_8002B0D0_LoadFileAlloc((s8 *) sp10, (void **) &g_main_8011F5A4_LevelHeader, 0);
        self = (u8 *) g_main_8011F5A4_LevelHeader;
    }

    hdr = g_main_8011F5A4_LevelHeader;

    g_main_8011F59C_SubDefs = (SubDefEntry *) (self + hdr->subDefsOff);
    g_main_8011F5A8_GroupCount = hdr->groupCount;
    g_main_8011F564_ObjCount = hdr->objCount;
    g_main_8011F560_SubDefCount = hdr->subDefCount;
    g_main_8011F664_RecordCount = hdr->recordCount;
    g_main_8011F24C_BitCount = hdr->bitCount;
    g_main_8011F568_unk = hdr->unk18;
    g_main_8011EEF8_WeaponDefs = (WeaponDef *) (self + hdr->weaponDefsOff);
    g_main_8011F590_GroupA = self + hdr->groupAOff;

    if (hdr->recordCount > 0) {
        g_main_8011F5B8_Records = self + hdr->recordsOff;
    } else {
        g_main_8011F5B8_Records = 0;
    }

    g_main_8011F254_HeaderTail = hdr->tail;
    g_main_8011F58C_GroupB = self + hdr->groupBOff;

    for (i = 0; i < g_main_8011F560_SubDefCount; i++) {
        SubDefEntry *e = &g_main_8011F59C_SubDefs[i];
        e->unk4 = self + (s32) (u64) e->unk4;
        e->unkC = self + (s32) (u64) e->unkC;
    }

    for (i = 0; i < g_main_8011F564_ObjCount; i++) {
        WeaponDef *def = &g_main_8011EEF8_WeaponDefs[i];
        if (def->unk2C != 0) {
            def->unk2C = self + (s32) (u64) def->unk2C;
        }
    }

    for (i = 0; i < g_main_8011F664_RecordCount; i++) {
        u8 *rec = g_main_8011F5B8_Records + i * 0x10;

        func_80080E80(i);

        *(u8 **) (rec + 0x8) = self + (s32) (u64) *(u8 **) (rec + 0x8);
        *(u8 **) (rec + 0xC) = self + (s32) (u64) *(u8 **) (rec + 0xC);

        if (*(s32 *) (rec + 0x4) > 0) {
            s32 *arr = *(s32 **) (rec + 0x8);
            for (j = 0; j < *(s32 *) (rec + 0x4); j++) {
                arr[j] += (s32) (u64) self;
            }
        }
    }

    if (g_main_8011F5A8_GroupCount > 0) {
        u8 *groupA = (u8 *) g_main_8011F590_GroupA;
        u8 *groupB = (u8 *) g_main_8011F58C_GroupB;

        for (i = 0; i < g_main_8011F5A8_GroupCount; i++) {
            s32 *aArr;

            *(u8 **) (groupA + 0x4) = self + (s32) (u64) *(u8 **) (groupA + 0x4);
            *(u8 **) (groupB + 0x4) = self + (s32) (u64) *(u8 **) (groupB + 0x4);

            aArr = *(s32 **) (groupA + 0x4);
            for (j = 0; j < *(s32 *) groupA; j++) {
                aArr[j] = (s32) (u64) (g_main_8011EEF8_WeaponDefs + aArr[j]);
            }

            groupA += 8;
            groupB += 8;
        }
    }
}
