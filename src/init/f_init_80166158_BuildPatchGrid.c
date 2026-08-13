#include "../../include/types.h"

/* Heavily unconfirmed - no caller context yet (only call site is
   func_80166498, itself not decompiled), so field/struct semantics below
   are best-effort guesses from the raw arithmetic alone.

   Builds a 14 (row) x 3 (col) x 2 (sub) grid of 0x30-byte "cell" records
   at arg0->unk20->unk108 + 0xBE0 ("base"). Each cell gets:
     - unk20 = 4 (constant)
     - unk24/28/2C/30 = four "corner" byte-pointers into an overlapping
       region of the same table, whose formula depends only on `col`
       (col 0 walks a 0x20-stride strip, col 1 a mix of 0x10/0x20-stride
       strips, col 2 a mix of 8-stride strips) - see the c0..c3 selection
       below. The middle two corners (28/2C) swap ends between sub 0 and
       sub 1.
     - unk10 = pointer into a companion 6-byte-stride/row (2-byte/col)
       table at base+0x1344.
     - unk14 = pointer 0x14 into a neighboring row-relative cell.
   Finally zeroes the word at `base` itself. */
void f_init_80166158_BuildPatchGrid(void *arg0) {
    void *p1 = *(void **) ((u8 *) arg0 + 0x20);
    void *p2 = *(void **) ((u8 *) p1 + 0x108);
    u8 *base = (u8 *) p2 + 0xBE0;

    s32 row;
    s32 rowAccum = 0;        /* += 0x120 per row */
    s32 rowOffA = 4;         /* += 0x120 per row */
    s32 rowOffB = 0xFC4;     /* += 0x40 per row */
    s32 rowIdxBase = 0x1344; /* += 6 per row */

    for (row = 0; row < 14; row++) {
        u8 *colBase = base + rowOffB;
        s32 col;
        s32 colOffA = 0;   /* += 0x60 per col */
        s32 colOffB = 0x30; /* += 0x60 per col */

        for (col = 0; col < 3; col++) {
            u8 *cellA = base + colOffA + rowAccum; /* sub 0 target */
            u8 *cellB = base + colOffB + rowAccum; /* sub 1 target */
            u8 *idxPtr = base + rowOffA + colOffA;
            s32 sub;

            u8 *pA1 = colBase; /* col 0, +0x20/sub */
            u8 *pT0 = colBase; /* col 1 corner0, +0x10/sub */
            u8 *pT2 = colBase; /* col 2 corner0, +8/sub */
            s32 oT8 = 0x20;    /* col 1 corner1, +0x10/sub */
            s32 oT9 = 0x10;    /* col 2 corner1, +8/sub */
            s32 oS0 = 0x30;    /* col 2 corner2, +8/sub */
            s32 oS1 = 0x20;    /* col 2 corner3, +8/sub */

            for (sub = 0; sub < 2; sub++) {
                u8 *c0, *c1, *c2, *c3;
                u8 *cell = (sub == 0) ? cellA : cellB;

                if (col == 0) {
                    c0 = pA1;
                    c1 = pA1 + 8;
                    c2 = pA1 + 0x18;
                    c3 = pA1 + 0x10;
                } else if (col == 1) {
                    c0 = pT0;
                    c1 = colBase + oT8;
                    c2 = colBase + oT8 + 8;
                    c3 = pT0 + 8;
                } else {
                    c0 = pT2;
                    c1 = colBase + oT9;
                    c2 = colBase + oS0;
                    c3 = colBase + oS1;
                }

                *(u32 *) (cell + 0x20) = 4;
                if (sub == 0) {
                    *(u8 **) (cell + 0x24) = c0;
                    *(u8 **) (cell + 0x28) = c1;
                    *(u8 **) (cell + 0x2C) = c2;
                    *(u8 **) (cell + 0x30) = c3;
                } else {
                    *(u8 **) (cell + 0x24) = c0;
                    *(u8 **) (cell + 0x28) = c3;
                    *(u8 **) (cell + 0x2C) = c2;
                    *(u8 **) (cell + 0x30) = c1;
                }

                *(u8 **) (cell + 0x10) = base + rowIdxBase + col * 2 + sub;
                *(u8 **) (cell + 0x14) = idxPtr + 0x14;

                idxPtr += 0x30;
                pA1 += 0x20;
                pT0 += 0x10;
                pT2 += 8;
                oT8 += 0x10;
                oT9 += 8;
                oS0 += 8;
                oS1 += 8;
            }

            colOffA += 0x60;
            colOffB += 0x60;
        }

        rowOffA += 0x120;
        rowIdxBase += 6;
        rowOffB += 0x40;
        rowAccum += 0x120;
    }

    *(u32 *) base = 0;
}
