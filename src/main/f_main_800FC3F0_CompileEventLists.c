#include "../../include/types.h"

/* Not in asm/800.s: 0x800FC3F0 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c.

   Extremely intricate: walks the same linked list f_main_800FC6A8_FindById searches
   (list head g_main_8012F408_unk, chained via +0x14), and for each node
   "compiles" up to *(+0x18) (s16 count) raw 0x24-byte event records at
   *(+0x20) against two lookup tables. Transcribed for control-flow
   accuracy from raw asm; field offsets are left as raw byte arithmetic
   (not a named struct) since their exact meaning is not understood beyond
   the layout observed here. */

extern SongTree *g_main_8012F408_unk;   /* list head, same one f_main_800FC6A8_FindById walks */

extern SongTree *f_main_800FC6A8_FindById(s32 id);   /* lookup table A = SongTree, matched by node->tag */
extern void *func_800FC6E8(s32 id);   /* lookup table B, matched by node field +0x10 (byte) */
extern s32 func_800FC910(s32 arg0);   /* guess: per-event finalizer */

void f_main_800FC3F0_CompileEventLists(void) {
    u8 *outer = (u8 *) g_main_8012F408_unk;

    while (outer != 0) {
        s16 count = *(s16 *) (outer + 0x18);
        s32 i;

        for (i = 0; i < count; i++) {
            u8 *arr = *(u8 **) (outer + 0x20);
            u8 *ev = arr + i * 0x24;
            u8 *matchA = 0;
            s32 flag = 0;
            s32 id0;

            if (*(u32 *) (ev + 0x20) & 1) {
                *(s32 *) (ev + 0x0) = *(u8 *) (ev + 0xC);
                *(s32 *) (ev + 0x4) = *(s32 *) (ev + 0x8);
            }

            id0 = *(s32 *) (ev + 0x0);

            if ((u32) (id0 - 7) < 2) {
                matchA = (u8 *) f_main_800FC6A8_FindById(*(s32 *) (ev + 4));
                flag = 1;
            }

            id0 = *(s32 *) (ev + 0x0);
            if ((u32) (id0 - 9) < 5) {
                matchA = func_800FC6E8(*(u8 *) (ev + 4));
                flag = 1;
            }

            if (matchA != 0) {
                u8 *dst = ev;
                s32 kind = *(s32 *) (dst + 0);

                if ((u32) (kind - 7) < 4) {
                    u16 limit = *(u16 *) (matchA + 0x18);
                    u8 subIdx = *(u8 *) (dst + 0x19);

                    if (subIdx < limit) {
                        u8 *sub = *(u8 **) (matchA + 0x20) + subIdx * 36;

                        *(u32 *) (dst + 4) = PTR_U32(matchA);
                        *(s32 *) (dst + 0) = *(s32 *) (sub + 0);
                        *(u8 *) (dst + 0xD) = *(u8 *) (sub + 0xD);

                        {
                            u8 subKind = *(u8 *) (ev + 0xC);

                            if (subKind == 7 || subKind == 9) {
                                *(u8 *) (dst + 0xE) = *(u8 *) (sub + 0xE);
                                *(u8 *) (dst + 0xF) = *(u8 *) (sub + 0xF);
                                *(u8 *) (dst + 0x10) = *(u8 *) (sub + 0x10);
                                *(u8 *) (dst + 0x11) = *(u8 *) (sub + 0x11);
                                *(u8 *) (dst + 0x12) = *(u8 *) (sub + 0x12);
                                *(u8 *) (dst + 0x13) = *(u8 *) (sub + 0x13);
                                *(u16 *) (dst + 0x14) = *(u16 *) (sub + 0x14);
                                *(u16 *) (dst + 0x16) = *(u16 *) (sub + 0x16);
                                *(u8 *) (dst + 0x18) = *(u8 *) (sub + 0x18);
                            }
                        }
                    } else {
                        *(s32 *) (dst + 4) = 0;
                    }
                } else {
                    *(s32 *) (dst + 0) = kind - 0xA;
                    *(u32 *) (ev + 4) = PTR_U32(matchA);
                }

                *(u32 *) (ev + 0x20) |= 1;
            } else {
                if (flag != 0) {
                    *(s32 *) (ev + 4) = 0;
                } else {
                    *(u32 *) (ev + 4) = PTR_U32(outer);
                }
                *(s32 *) (ev + 0x1C) = func_800FC910(*(s32 *) (ev + 0xC));
            }
        }

        *(u32 *) (outer + 8) |= 2;
        outer = *(u8 **) (outer + 0x14);
    }
}
