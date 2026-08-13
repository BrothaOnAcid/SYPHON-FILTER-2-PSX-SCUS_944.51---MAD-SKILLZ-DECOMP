#include "../../include/types.h"

/* Initializes a 30-entry (stride 0x30) record table living at
   ((arg0->unk20)->unk108) + 0x1F78. Each record[i]:
     +0x00, +0x04: s32 = 0
     +0x0C: pointer into a separate byte-indexed table at
            recordBase + 0x964 + i (only that one byte gets zeroed there)
     +0x10: self-pointer to record[i]+0x14
     +0x14, +0x16, +0x18: u16 = 0
     +0x1C: s32 = 0
     +0x20..+0x2C: 4 pointers into a per-record 0x20-byte sub-table at
            recordBase + 0x5A4 + i*0x20 (stride 8 each); each 8-byte
            sub-entry has its first 3 u16s (bytes 0,2,4) zeroed, byte 6-7
            left untouched.
   Finally zeroes the s32 right at the very start of the whole table
   (recordBase + 0x0) - redundant with record[0]'s own +0x0 write above but
   matched from the asm as-is. */
void f_init_801663E0_InitTrackTable(void *arg0) {
    u8 *ctx = *(u8 **) ((u8 *) arg0 + 0x20);
    u8 *base = *(u8 **) (ctx + 0x108);
    u8 *recordBase = base + 0x1F78;
    s32 i, k;

    for (i = 0; i < 0x1E; i++) {
        u8 *rec = recordBase + i * 0x30;
        u8 *sub = recordBase + 0x5A4 + i * 0x20;

        *(s32 *) (rec + 0x1C) = 0;

        for (k = 0; k < 4; k++) {
            u8 *slot = sub + k * 8;

            *(void **) (rec + 0x20 + k * 4) = slot;
            *(s16 *) (slot + 0x0) = 0;
            *(s16 *) (slot + 0x2) = 0;
            *(s16 *) (slot + 0x4) = 0;
        }

        *(void **) (rec + 0x10) = rec + 0x14;
        *(s16 *) (rec + 0x14) = 0;
        *(s16 *) (rec + 0x16) = 0;
        *(s16 *) (rec + 0x18) = 0;
        *(void **) (rec + 0xC) = recordBase + 0x964 + i;
        *(recordBase + 0x964 + i) = 0;
        *(s32 *) (rec + 0x0) = 0;
        *(s32 *) (rec + 0x4) = 0;
    }

    *(s32 *) recordBase = 0;
}
