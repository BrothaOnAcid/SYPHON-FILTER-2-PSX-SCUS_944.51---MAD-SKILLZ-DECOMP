#include "../../include/types.h"

/* Opaque blobs (main-module data), only their addresses are used here. */
extern u8 D_800ECA5C[];
extern u8 D_800EE92C[];

extern void *D_8011F6FC; /* -> D_800ECA5C; stride-0x30 entry array walked below */
extern void *D_8011F700; /* -> D_800EE92C; stride-0x20 entry array walked below */

extern s32 D_8012C720; /* end (highest address) of a 185-word (0x2E4-byte) scratch block, zeroed downward */
extern void *D_8012C440[]; /* index table: entry-id -> stride-0x20 entry pointer (from D_8011F700's list) */
extern void *D_8012C1EC[]; /* index table: entry-id -> stride-0x30 entry pointer (from D_8011F6FC's list) */
extern s16 *D_80113050[];  /* index table: value2-id -> variable-length s16 sequence, -1-terminated */

/* Zeroes a 185-word block ending at D_8012C720 (inclusive, walked backward),
   then points D_8011F6FC/D_8011F700 at their fixed data blobs.

   Phase 1: walks the stride-0x20 entry list at D_8011F700 (each entry's
   first s16 is an id, -1 terminates the list) and records each entry's
   address into D_8012C440[id].

   Phase 2: walks 0x95 (149) stride-0x30 entries starting at D_8011F6FC.
   For each entry: records its address into D_8012C1EC[entry's first s16].
   If the entry's second s16 (value2) is -1, its +0x9 byte is cleared.
   Otherwise value2 indexes D_80113050 to get a -1-terminated s16 sequence;
   if that sequence's first element is negative, +0x9 is left untouched;
   otherwise the sequence is scanned (stride 0x14 per step, i.e. every 10th
   s16) until a negative element is found, and the resulting step count is
   stored into the entry's +0x9 byte. */
void f_init_801623B0_BuildIndexTables(void) {
    s32 *p;
    s32 i;
    s16 *listB;
    s16 *listA;

    p = &D_8012C720;
    for (i = 0xB8; i >= 0; i--, p--) {
        *p = 0;
    }

    D_8011F6FC = D_800ECA5C;
    D_8011F700 = D_800EE92C;

    listB = (s16 *) D_8011F700;
    if (*listB != -1) {
        u8 *entry = (u8 *) listB;

        do {
            s16 id = *(s16 *) entry;

            D_8012C440[id] = entry;
            entry += 0x20;
        } while (*(s16 *) entry != -1);
    }

    listA = (s16 *) D_8011F6FC;
    for (i = 0; i < 0x95; i++) {
        u8 *entry = (u8 *) listA + i * 0x30;
        s16 id = *(s16 *) entry;
        s16 value2 = *(s16 *) (entry + 2);

        D_8012C1EC[id] = entry;

        if (value2 == -1) {
            *(u8 *) (entry + 9) = 0;
        } else {
            s16 *seq = D_80113050[value2];

            if (*seq >= 0) {
                s32 step = 1;

                while (*(s16 *) ((u8 *) seq + step * 0x14) >= 0) {
                    step++;
                }
                step--;
                *(u8 *) (entry + 9) = (u8) step;
            }
        }
    }
}
