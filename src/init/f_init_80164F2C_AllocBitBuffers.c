#include "../../include/types.h"

extern s32 g_main_8011F24C_BitCount; /* not yet decompiled: bit count, see f_init_80158E3C's g_main_8011F24C_BitCount assignment */
extern s32 D_8011F250; /* not yet decompiled: byte size of each buffer allocated below */

extern void *f_main_80025AD0_AllocDown(s32 size);
extern void f_main_80025E40_MemsetWords(void *dst, u8 val, u32 size);

/* No-ops if g_main_8011F24C_BitCount is 0. Otherwise computes the
   word-aligned byte size to hold that many bits (round-up-to-32,
   divide-by-8: ((bits + 0x1F) >> 5) * 4, with the classic negative-safe
   rounding via a +0x3E variant when bits+0x1F would go negative), stashes
   it in D_8011F250, and allocates + zeroes two such buffers at
   arg0->unk20->unk540 and ->unk544 (presumably a pair of bitsets). */
void f_init_80164F2C_AllocBitBuffers(void *arg0) {
    s32 bits = g_main_8011F24C_BitCount;

    if (bits != 0) {
        void *obj = *(void **) ((u8 *) arg0 + 0x20);
        s32 words = (bits + 0x1F) >> 5;

        if (bits + 0x1F < 0) {
            words = (bits + 0x3E) >> 5;
        }
        D_8011F250 = words * 4;

        *(void **) ((u8 *) obj + 0x540) = f_main_80025AD0_AllocDown(D_8011F250);
        f_main_80025E40_MemsetWords(*(void **) ((u8 *) obj + 0x540), 0, D_8011F250);

        obj = *(void **) ((u8 *) arg0 + 0x20);
        *(void **) ((u8 *) obj + 0x544) = f_main_80025AD0_AllocDown(D_8011F250);
        f_main_80025E40_MemsetWords(*(void **) ((u8 *) obj + 0x544), 0, D_8011F250);
    }
}
