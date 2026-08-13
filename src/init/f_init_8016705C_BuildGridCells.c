#include "../../include/types.h"

extern void *f_main_80025AD0_AllocDown(s32 size);
extern void func_80012A84(void *widget, s32 a1, s32 a2, s32 a3, s32 sp10, s32 sp14, void *entry, s32 sp1c); /* not yet decompiled */

extern s32 D_80116BC0[4]; /* not yet decompiled: per-column scale factor */
extern s32 D_80116BD0[4]; /* not yet decompiled: per-column raw count (reduced mod `divisor`) */
extern u8 D_80116BF0[];   /* not yet decompiled: per-cell byte table, indexed by a running 0x84-stride byte offset */
extern u8 D_80116BF4[];   /* not yet decompiled: per-cell data table, same indexing as D_80116BF0 */
extern u8 D_8013C9C0[];   /* not yet decompiled: per-column output x, stride 4 (s16 used) */
extern u8 D_8013C9D0[];   /* not yet decompiled: per-column output y, stride 4 (s16 used) */

/* Heavily unconfirmed - large grid/sprite-cell setup, no caller context
   decompiled yet. Allocates a 0xAA0-byte buffer at arg0+0x1D4 (a leading
   pointer word, zeroed, plus 30 zeroed 0xC-byte trailer slots).

   Builds a 3 (outer) x 4 (middle) grid: each cell reads
   D_80116BD0[col] (reduced mod `divisor`), looks up
   `entries[cellIndex]`'s +0x14/+0x16 halfwords scaled by
   D_80116BC0[col], derives two angle-like values (each further halved
   into quarter values), writes the results into
   D_8013C9C0/D_8013C9D0 (offset by arg0->unkC's +0x4/+0x6 halved), then -
   for the per-cell count read from D_80116BF0 (indexed by the running
   0x84-stride byte offset) - fills that many 0x20-byte sub-records
   (selected via a switch on the count: 1/2/4, anything else is a no-op)
   and dispatches each through func_80012A84. */
void f_init_8016705C_BuildGridCells(void *arg0, s32 divisor, u8 *entries) {
    u8 *a = (u8 *) arg0;
    u8 *buf = (u8 *) f_main_80025AD0_AllocDown(0xAA0);
    u8 *slot;
    s32 i;
    s32 outer;
    u8 *rowBase;

    *(void **) (a + 0x1D4) = buf;
    *(void **) buf = 0;

    slot = buf + 0xA90;
    for (i = 0x1D; i >= 0; i--) {
        *(s32 *) slot = 0;
        slot -= 0xC;
    }

    rowBase = buf + 4;
    for (outer = 0; outer < 3; outer++) {
        u8 *colPtr = rowBase;
        u8 *cellPtr = rowBase + 0x10;
        s32 t4 = 0;
        s32 col;

        for (col = 0; col < 4; col++) {
            s32 count = *(s32 *) (D_80116BF0 + t4);
            u8 *cellData = D_80116BF4 + t4;
            s32 remCount = D_80116BD0[col];
            s32 scale = D_80116BC0[col];
            u8 *entry;
            s32 a1, lo;
            s16 s5, s3, s2, t3;
            s32 halfX, halfY;

            *(u8 **) colPtr = cellPtr;

            if (remCount >= divisor)
                remCount %= divisor;

            entry = entries + remCount * 0x2C;
            a1 = *(s16 *) (entry + 0x14) * scale;
            if (a1 < 0)
                a1 += 0x1FFF;
            lo = *(s16 *) (entry + 0x16) * scale;

            s5 = (s16) (a1 >> 13);
            s3 = (s16) (a1 >> 14);
            s2 = (s16) ((lo * 0x140) / 3145728);
            t3 = (s16) ((lo * 0x140) / 6291456);

            halfX = s5;
            if (halfX < 0)
                halfX += 0x1FFF;
            *(s16 *) (D_8013C9C0 + col * 4) = (s16) (halfX >> 13) +
                *(s16 *) (*(u8 **) (a + 0xC) + 0x4) / 2;

            halfY = s2;
            if (halfY < 0)
                halfY += 0x1FFF;
            *(s16 *) (D_8013C9D0 + col * 4) = (s16) (halfY >> 13) +
                *(s16 *) (*(u8 **) (a + 0xC) + 0x6) / 2;

            if (count > 0) {
                s16 negS3 = (s16) -s3;
                s16 negS5 = (s16) -s5;
                s16 negS2 = (s16) -s2;
                s16 negT3 = (s16) -t3;
                u8 *rec = cellPtr;
                u8 *sub = cellPtr; /* separate walker over the 0x30-stride
                                       sub-record array starting at cellPtr */
                s32 n = 0;

                do {
                    if (count == 2) {
                        *(s16 *) (rec + 0x0) = 0;
                        *(s16 *) (rec + 0x2) = s2;
                        *(s16 *) (rec + 0x8) = (n != 0) ? negS5 : s5;
                        *(s16 *) (rec + 0xA) = s2;
                        *(s16 *) (rec + 0x10) = 0;
                        *(s16 *) (rec + 0x12) = negS2;
                        *(s16 *) (rec + 0x18) = (n != 0) ? negS5 : s5;
                        *(s16 *) (rec + 0x1A) = negS2;
                    } else if (count == 4) {
                        *(s16 *) (rec + 0x0) = 0;
                        *(s16 *) (rec + 0x2) = 0;
                        *(s16 *) (rec + 0x8) = (n != 0 && n != 2) ? negS5 : s5;
                        *(s16 *) (rec + 0xA) = 0;
                        *(s16 *) (rec + 0x10) = 0;
                        *(s16 *) (rec + 0x12) = (n < 2) ? negS2 : s2;
                        *(s16 *) (rec + 0x18) = (n != 0 && n != 2) ? negS5 : s5;
                        *(s16 *) (rec + 0x1A) = (n < 2) ? negS2 : s2;
                    } else if (count == 1) {
                        *(s16 *) (rec + 0x0) = negS3;
                        *(s16 *) (rec + 0x2) = t3;
                        *(s16 *) (rec + 0x8) = s3;
                        *(s16 *) (rec + 0xA) = t3;
                        *(s16 *) (rec + 0x10) = negS3;
                        *(s16 *) (rec + 0x12) = negT3;
                        *(s16 *) (rec + 0x18) = s3;
                        *(s16 *) (rec + 0x1A) = negT3;
                    }
                    /* any other count value: no fields written, but the
                       dispatch/bookkeeping below still runs (matches the
                       original asm's switch-with-fallthrough shape) */

                    func_80012A84(sub, 0, 0, 0, 0, 0, entry, 1);

                    rec += 0x20;
                    n++;
                    *(s32 *) sub = 0;
                    *(u8 *) (sub + 0xF) |= 2;
                    sub += 0x30;
                } while (n < count);
            }

            colPtr += 4;
            cellPtr += 0x30;
            t4 += 0x84;
        }

        rowBase += 0x310;
    }
}
