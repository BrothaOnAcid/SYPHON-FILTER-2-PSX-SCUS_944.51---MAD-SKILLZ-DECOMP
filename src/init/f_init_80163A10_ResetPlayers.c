#include "../../include/types.h"

extern Player g_main_8012A574_Player[2]; /* same symbol as the `AmmoUser *`/`void *` used
                                             elsewhere - here its address is taken directly,
                                             confirming it's an in-place struct array */

extern void *f_main_800F4190_memset(void *dst, s32 fillValue, s32 size);

/* Heavily unconfirmed - resets both players' Player state block: zeroes
   the whole 0x55C-byte record, then stamps a long list of individual
   fields/small arrays back to fixed reset values (mostly -1 "unset"
   sentinels, some 0/1 flags and zeroed counters). No caller context
   decompiled yet to firm up field meaning - see the Player typedef in
   types.h for what's guessed so far. */
void f_init_80163A10_ResetPlayers(void) {
    s32 inst;

    for (inst = 0; inst < 2; inst++) {
        Player *p = &g_main_8012A574_Player[inst];
        s32 i;

        f_main_800F4190_memset(p, 0, sizeof(Player));

        p->base.unk14 = -1;
        p->unkCC = 0;
        p->unk200 = 1;
        p->unk204 = 0;
        p->unk208 = 0;
        p->unk20C = 0;
        p->unk218 = -1;
        p->unk21A = -1;
        p->unk264 = 0;
        p->unk394 = 0;
        p->unk398 = 1;
        p->unk399 = 1;
        p->unk530 = -1;
        p->unk534 = -1;
        p->unk538 = -1;
        p->unk54C = -1;
        p->unk550 = -1;
        p->unk554 = -1;
        p->unk558 = -1;
        p->base.flags = (u8 *) (u64) (PTR_U32(p->base.flags) | 4); /* tag bit set
                                                                        directly on the
                                                                        pointer value */
        p->flags34 &= 0xFFFEFFFF;

        for (i = 0; i < 4; i++) {
            *(s16 *) ((u8 *) p + 0x210 + i * 2) = -1;
        }

        p->unk39C = 0;
        p->unk3C0 = 0;

        for (i = 0; i < 8; i++) {
            p->unk3B0[i] = 0;
            p->unk3A0[i] = -1;
            p->unk3C4[i] = -1;
            p->unk414[i] = -1;
            p->unk3E4[i] = -1;
            p->unk3F4[i] = -1;
            p->unk404[i] = -1;
        }

        p->unk424 = 0;

        for (i = 0; i < 10; i++) {
            p->unk428[i] = -1;
            p->unk464[i] = 0;
            p->unk450[i] = 0;
            p->unk43C[i] = 0;
            p->unk45A[i] = 0;
        }

        for (i = 0; i < 0x23; i++) {
            p->unk470[i] = -1;
            p->unk4FC[i] = 0;
        }

        p->unk520 = -1;
        p->unk524 = 0;
        p->unk528 = -1;
        p->unk52A = -1;
    }
}
