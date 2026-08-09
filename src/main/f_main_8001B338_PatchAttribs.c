#include "../../include/types.h"

/* Patches packed bitfields in an array of 16-byte records starting at
   base+0x24. For each of the `count` records (count = (*(u32*)(base+8) >> 2)
   - 1, so the array holds count+1 entries):
     record[+2] = (record[+2] & 0x3F) | (hi << 6)   ; low 6 bits kept
     record[+6] = (record[+6] & 0xFFE0) | lo        ; low 5 bits overwritten
   where `lo` and `hi` are the raw caller values (hi is shifted by 6 first).
   Assumes count >= 1 (an empty list would loop forever). */
void f_main_8001B338_PatchAttribs(u8 *base, u32 lo, u32 hi) {
    u8 *p = base + 0x24;
    s32 n = ((*(u32 *) (base + 8)) >> 2) - 1;

    do {
        *(u16 *) (p + 2) = (u16) ((*(u16 *) (p + 2) & 0x3F) | (hi << 6));
        *(u16 *) (p + 6) = (u16) ((*(u16 *) (p + 6) & 0xFFE0) | lo);
        p += 0x10;
    } while (--n != 0);
}
