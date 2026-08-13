#include "../../include/types.h"

extern TypeDefCache g_main_80168AA8_TypeDefCache;

extern ListNode *f_main_80025C3C_AddNode(ListHead *head, void *owner);

/* arg0: object with a mode byte at +0x1 (cleared except bit 0x80), a bitfield
   at +0x4 (low byte = cache index, nibble 0xF000 = "level", both cleared
   here), and a WldRes* at +0x8 whose ->typeDef is the cache key.

   Looks up arg0's typeDef in g_main_80168AA8_TypeDefCache (capped at 7
   live entries): on a hit, stores the matched index into arg0's +0x4 low
   byte and links arg0 onto values[i]'s node list via
   f_main_80025C3C_AddNode. On a miss, if the cache isn't full (count < 7),
   appends a new entry (key = typeDef, empty node list) at the next free
   index, stores that index into arg0's +0x4 low byte, and links arg0 onto
   the fresh (empty) list the same way - and bumps count.

   NOTE: matches a real quirk in the compiled asm - the "did we skip
   inserting" flag is only ever explicitly set on a cache hit (0) or when
   the cache was already over-capacity (1); for an ordinary miss (count in
   1..7) it's read without ever being written in this call, i.e. it carries
   over the register's leftover value. Reproduced here with `skipInsert`
   left uninitialized on that path - not a bug, matched. */
void f_init_8015ECC0_CacheTypeDefSlot(void *arg0) {
    u8 skipInsert;
    s16 count, cappedCount, i;
    void *typeDef;

    *((u8 *) arg0 + 1) &= 0x80;
    *(u32 *) ((u8 *) arg0 + 4) &= 0xFFFF0F00;

    count = g_main_80168AA8_TypeDefCache.count;
    cappedCount = count;
    if (count > 7) {
        skipInsert = 1;
        cappedCount = 7;
    }

    typeDef = (*(WldRes **) ((u8 *) arg0 + 8))->typeDef;

    for (i = 0; i < cappedCount; i++) {
        if (g_main_80168AA8_TypeDefCache.keys[i] == typeDef) {
            *(u32 *) ((u8 *) arg0 + 4) = (*(u32 *) ((u8 *) arg0 + 4) & ~0xFF) | (i & 0xFF);
            f_main_80025C3C_AddNode(&g_main_80168AA8_TypeDefCache.values[i], arg0);
            skipInsert = 0;
            goto done;
        }
    }

done:
    if (!skipInsert) {
        if (g_main_80168AA8_TypeDefCache.count < 7) {
            s16 idx = g_main_80168AA8_TypeDefCache.count;

            *(u32 *) ((u8 *) arg0 + 4) = (*(u32 *) ((u8 *) arg0 + 4) & ~0xFF) | (idx & 0xFF);
            g_main_80168AA8_TypeDefCache.keys[idx] = typeDef;
            g_main_80168AA8_TypeDefCache.values[idx].first = 0;
            f_main_80025C3C_AddNode(&g_main_80168AA8_TypeDefCache.values[idx], arg0);
        }
        g_main_80168AA8_TypeDefCache.count++;
    }
}
