#include "../../include/types.h"

extern s32 func_8002D4B4(void *a0, s32 *out);       /* not yet decompiled */
extern void *func_800378C4(void *a0, s32 a1);        /* not yet decompiled */
extern void func_80037940(void *a0, s32 a1);          /* not yet decompiled */
extern void func_80037948(void *a0, s32 a1);          /* not yet decompiled */
extern void func_80100200(void *a0, s16 *a1, s16 a2, s16 a3); /* not yet decompiled */

extern void *D_8011F6E0;   /* set by f_init_8015B96C_LoadWithFlag */
extern u8 D_80158C38;      /* not yet decompiled */
extern u8 D_80158C44;      /* not yet decompiled */

extern u8 D_80129FD4;      /* not yet decompiled: 6-entry table, stride 0x20, header at -8 */
extern s16 D_8012A08C;     /* not yet decompiled */
extern s16 D_8012A090;     /* not yet decompiled */
extern s16 D_8012A094;     /* not yet decompiled */

extern s16 D_8012A16C;     /* not yet decompiled: 13-entry table, stride 8 */
extern s16 D_8012A19C;
extern s16 D_8012A19E;
extern s16 D_8012A1A0;
extern s16 D_8012A1A2;

extern s16 D_8012A0AC;     /* not yet decompiled: 6 fixed camera-path records, stride 0x20 */
extern s16 D_8012A0B0;
extern s16 D_8012A0B4;
extern s16 D_8012A0CC;
extern s16 D_8012A0D0;
extern s16 D_8012A0EC;
extern s16 D_8012A0F0;
extern s16 D_8012A10C;
extern s16 D_8012A110;
extern s16 D_8012A12C;
extern s16 D_8012A130;
extern s16 D_8012A14C;
extern s16 D_8012A150;
extern s16 D_8012A1A4;

/* Heavily unconfirmed camera/animation table setup: for each of two
   sub-objects (arg0->unk1320[i]) attaches a resource looked up via
   func_8002D4B4(D_80158C38/D_80158C44) and, if found, ORs 0x20000000
   into that resource's +0x28 flags via func_800378C4. Then, if
   D_8011F6E0->unk54 is non-zero, fills two large fixed-layout tables
   (D_80129FD4 array and D_8012A16C/D_8012A0B4 arrays) from
   D_8011F6E0->unk54/unk56/unk58/unk5A and calls func_80100200 to
   populate each record, finally calling func_80037948(arg0, hasTable). */
void f_init_80164ACC_SetupCameraTables(void *arg0) {
    u8 *slot;
    s32 got;
    s32 any;
    s32 i;
    void *res;
    u8 *hdr;
    s16 count;
    u16 baseA;
    u16 baseB;
    u16 baseC;
    s16 sizeA;
    s16 idx;
    u8 *entry;
    u8 *tbl;
    u8 *rec;
    u8 *p;

    any = 0;
    got = 0;
    if (arg0 == 0) {
        return;
    }

    slot = (u8 *) arg0;
    for (i = 0; i < 2; i++) {
        func_8002D4B4(i == 0 ? &D_80158C38 : &D_80158C44, &got);
        if (got != 0) {
            res = func_800378C4(arg0, got);
            *(void **) (slot + 0x1320) = res;
            hdr = *(void **) ((u8 *) res + 0x10);
            any = 1;
            *(s32 *) (hdr + 0x28) |= 0x20000000;
        }
        slot += 4;
    }
    func_80037940(arg0, (any & 0xFF) != 0 ? 1 : -1);

    count = *(s16 *) ((u8 *) D_8011F6E0 + 0x54);
    if (count != 0) {
        baseA = *(u16 *) ((u8 *) D_8011F6E0 + 0x56);
        baseB = *(u16 *) ((u8 *) D_8011F6E0 + 0x58);
        baseC = *(u16 *) ((u8 *) D_8011F6E0 + 0x5A);
        sizeA = count + 0x3F;

        *(&D_8012A16C + 0x0) = count;
        *(&D_8012A16C + 0x5) = baseA + 0x40;   /* D_8012A176 */
        *(&D_8012A16C + 0x9) = baseA + 0x40;   /* D_8012A17E */
        *(&D_8012A16C + 0x1) = baseA;          /* D_8012A16E */
        *(&D_8012A16C + 0x2) = 1;              /* D_8012A170 */
        *(&D_8012A16C + 0x3) = 0x40;           /* D_8012A172 */
        *(&D_8012A16C + 0x4) = sizeA;          /* D_8012A174 */
        *(&D_8012A16C + 0x6) = 1;              /* D_8012A178 */
        *(&D_8012A16C + 0x7) = 0x40;           /* D_8012A17A */
        *(&D_8012A16C + 0x8) = count;          /* D_8012A17C */
        *(&D_8012A16C + 0xA) = 1;              /* D_8012A180 */
        *(&D_8012A16C + 0xB) = 0x40;           /* D_8012A182 */
        *(&D_8012A16C + 0xC) = count;          /* D_8012A184 */
        *(&D_8012A16C + 0xD) = baseA + 0x80;   /* D_8012A186 */
        *(&D_8012A16C + 0xE) = 1;              /* D_8012A188 */
        *(&D_8012A16C + 0xF) = 0x40;           /* D_8012A18A */
        *(&D_8012A16C + 0x10) = sizeA;                /* D_8012A18C */
        *(&D_8012A16C + 0x11) = (s16) (baseA + 0xC0);  /* D_8012A18E */
        *(&D_8012A16C + 0x12) = 1;                    /* D_8012A190 */
        *(&D_8012A16C + 0x13) = 0x40;                 /* D_8012A192 */
        *(&D_8012A16C + 0x14) = count;                /* D_8012A194 */
        *(&D_8012A16C + 0x15) = (s16) (baseA + 0xC0);  /* D_8012A196 */
        *(&D_8012A16C + 0x16) = 1;                    /* D_8012A198 */
        *(&D_8012A16C + 0x17) = 0x40;                 /* D_8012A19A */

        entry = (u8 *) &D_80129FD4 - 8;
        tbl = (u8 *) &D_8012A16C;
        rec = (u8 *) &D_80129FD4;
        for (i = 0; i < 6; i++) {
            func_80100200(rec, (s16 *) tbl, (s16) (baseB + i), (s16) baseC);
            *(s32 *) entry = 0;
            *(s16 *) (entry + 4) = 1;
            entry += 0x20;
            tbl += 8;
            rec += 0x20;
        }

        idx = 7;
        D_8012A1A0 = 0x3F;
        D_8012A1A2 = 0x100;
        D_8012A19C = (u16) count + 1;
        D_8012A19E = baseA;
        func_80100200(&D_8012A094, &D_8012A19C, (s16) count, (s16) baseA);

        p = (u8 *) &D_8012A16C + 0x38;
        for (; idx < 0xD; idx++) {
            *(s16 *) (p + 0) = (s16) (baseB - 7 + idx);
            *(s16 *) (p + 4) = 1;
            *(s16 *) (p + 6) = 0x40;
            *(s16 *) (p + 2) = baseC;
            p += 8;
        }

        func_80100200(&D_8012A0B4, &D_8012A1A4, count + 0x3F, (s16) baseA + 0x40);
        D_8012A0B0 = 1;
        D_8012A0AC = 0;
        func_80100200((&D_8012A0B4) + 0x10, (&D_8012A1A4) + 4, count + 0x3E, (s16) baseA + 0x40);
        D_8012A0D0 = 1;
        D_8012A0CC = 0;
        func_80100200((&D_8012A0B4) + 0x20, (&D_8012A1A4) + 8, count + 0x3F, (s16) baseA);
        D_8012A0F0 = 1;
        D_8012A0EC = 0;
        func_80100200((&D_8012A0B4) + 0x30, (&D_8012A1A4) + 0xC, count + 0x3F, (s16) baseA + 0xC0);
        D_8012A110 = 1;
        D_8012A10C = 0;
        func_80100200((&D_8012A0B4) + 0x40, (&D_8012A1A4) + 0x10, count + 0x3E, (s16) baseA + 0xC0);
        D_8012A130 = 1;
        D_8012A12C = 0;
        func_80100200((&D_8012A0B4) + 0x50, (&D_8012A1A4) + 0x14, count + 0x3F, (s16) baseA + 0x80);
        D_8012A150 = 1;
        D_8012A14C = 0;
        func_80037948(arg0, 1);
    } else {
        func_80037948(arg0, 0);
    }
}
