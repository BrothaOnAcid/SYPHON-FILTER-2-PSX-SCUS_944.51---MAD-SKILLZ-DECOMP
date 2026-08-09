#include "../../include/types.h"

/* Not in asm/800.s: 0x800FD020 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c.

   Takes 3 args, not the 2 originally guessed in
   f_main_800F9F68_PlaySongNode's extern (fixed there too, using the
   caller's existing `*(u16*)(entry+0xE) & 1` expression for the new 3rd
   arg - same value it already computes into buf2+0x14). */

extern Rec48 g_main_801412C0_Rec48[];
extern s32 f_main_80104D30_GetSeqCount(void);
extern s32 f_main_800FBF64_ScaleByVolumeCurve(s16 raw, s32 index);
extern AudioSeqState g_main_8012F41C_AudioSeq;

/* guess: builds an SPU command packet {mask, 0x000600EF, panL, panR, 0, 0,
   ..., Rec48-sourced fields...} and transmits it - the true low-level SPU
   primitive, left extern per "except psyq calls". */
extern void func_80105C8C(void *cmdPacket);

/* guess: activates channel `index`'s record: stamps unk00=1/unk04=fresh
   sequence count/unk08=src->0x20, bulk-copies 0x34 bytes from `src` into
   the record starting at +0xC (so the record's own +0xC.. fields mirror
   the source), then - only if the source's header field (+0x4) is < 17 -
   recomputes the volume-scaled pan from the just-copied +0xC/+0xE values
   and sends an SPU command packet, updates
   g_main_8012F41C_AudioSeq.flagsC's bit for this channel (set if the
   record's +0x20 bit 0 is set, else cleared), and if `flag` is set also
   sets the channel's bit in AudioSeq.mask. */
void f_main_800FD020_ActivateChannelRecord(s32 index, void *src, s32 flag) {
    Rec48 *rec = &g_main_801412C0_Rec48[index];
    u32 mask = 1u << index;
    u8 *raw = (u8 *) rec;
    u8 *s = (u8 *) src;

    rec->unk00 = 1;
    rec->unk04 = f_main_80104D30_GetSeqCount();
    rec->unk08 = *(u32 *) (s + 0x20);

    {
        s32 i;

        for (i = 0; i < 0x34; i += 4) {
            *(u32 *) (raw + 0xC + i) = *(u32 *) (s + i);
        }
    }

    if (*(s16 *) (s + 4) >= 0x11) {
        return;
    }

    {
        u8 packet[0x40];
        s16 panL = f_main_800FBF64_ScaleByVolumeCurve(rec->unk0C, rec->unk30);
        s16 panR = f_main_800FBF64_ScaleByVolumeCurve(rec->unk0E, rec->unk30);

        *(u32 *) (packet + 0x00) = mask;
        *(u32 *) (packet + 0x04) = 0x000600EF;
        *(s16 *) (packet + 0x08) = panL;
        *(s16 *) (packet + 0x0A) = panR;
        *(u16 *) (packet + 0x0C) = 0;
        *(u16 *) (packet + 0x0E) = 0;
        *(u16 *) (packet + 0x16) = *(u16 *) (raw + 0x18);
        *(u16 *) (packet + 0x18) = *(u16 *) (raw + 0x1A);
        *(u32 *) (packet + 0x1C) = *(u32 *) (raw + 0x1C);
        *(u16 *) (packet + 0x3A) = *(u16 *) (raw + 0x24);
        *(u16 *) (packet + 0x3C) = *(u16 *) (raw + 0x26);

        func_80105C8C(packet);
    }

    if (rec->unk20 & 1) {
        g_main_8012F41C_AudioSeq.flagsC |= mask;
    } else {
        g_main_8012F41C_AudioSeq.flagsC &= ~mask;
    }

    if (flag != 0) {
        g_main_8012F41C_AudioSeq.mask |= mask;
    }
}
