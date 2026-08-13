#include "../../include/types.h"

extern u8 D_8012EC3C[]; /* not yet decompiled: 0x10-byte-stride output table, see f_init_80165F00_ScanEntryTable */

/* Packs a 16-byte record into D_8012EC3C[index] from fields of `src` (a
   frame/keyframe-shaped struct: u16s at +0x10/+0x12/+0x14/+0x16/+0x1C/+0x1E,
   plus a 2-bit type/count nibble at +0xC & 3). Bit-packing details are
   unconfirmed beyond matching the asm 1:1 - looks like a quantized
   vector/angle format shared with animation compression. */
void f_init_80165E48_PackFrame(void *src, s32 index) {
    u8 *out = D_8012EC3C + index * 0x10;
    u32 type = *((u8 *) src + 0xC) & 3;
    u16 f12 = *(u16 *) ((u8 *) src + 0x12);
    u16 f10 = *(u16 *) ((u8 *) src + 0x10);
    u32 packed = (type << 7) | ((f12 >> 4) & 0x10) | ((f10 & 0x3FF) >> 6) | ((f12 & 0x200) << 2);
    u32 shift = (2 - type) & 0x1F; /* matches MIPS sllv's shift-amount truncation for the type==3 case */

    *(u32 *) (out + 0x0) = type << 24;
    *(s16 *) (out + 0x4) = (s16) (packed & 0x1F);
    *(s8 *) (out + 0x6) = (s8) ((u32) (*(s16 *) ((u8 *) src + 0x10) - ((packed & 0xF) << 6)) << shift);
    *(s8 *) (out + 0x7) = (s8) f12;
    *(s16 *) (out + 0x8) = (s16) (*(u16 *) ((u8 *) src + 0x14) << shift);
    *(s16 *) (out + 0xA) = (s16) *(u16 *) ((u8 *) src + 0x16);
    *(s16 *) (out + 0xC) = (s16) *(u16 *) ((u8 *) src + 0x1C);
    *(s16 *) (out + 0xE) = (s16) *(u16 *) ((u8 *) src + 0x1E);
}
