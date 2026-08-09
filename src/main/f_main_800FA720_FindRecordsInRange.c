#include "../../include/types.h"

/* Not in asm/800.s: 0x800FA720 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c.

   Referenced from f_main_800F9F68_PlaySongNode's comment as "fills a small
   list of entries via func_800FA720". `handler` points at a sub-table
   (own +0x24: byte count `n` + record-array pointer, stride 8, indexed by
   `index`); each 0x18-byte record's bytes at +6/+7 form an inclusive
   [min,max] range - records where `value` falls in range get appended to
   `outList`. Returns the number of matches. */
s32 f_main_800FA720_FindRecordsInRange(u8 *handler, s32 index, s32 value, void **outList) {
    s16 count = *(s16 *) (handler + 0x1A);
    u8 *entry;
    u8 n;
    u8 *rec;
    s32 matches = 0;
    s32 i;

    if (index >= count) {
        return 0;
    }

    entry = *(u8 **) (handler + 0x24) + index * 8;
    n = *entry;
    rec = *(u8 **) (entry + 4);

    for (i = 0; i < n; i++) {
        if (value >= rec[6] && rec[7] >= value) {
            outList[matches] = rec;
            matches++;
        }
        rec += 0x18;
    }

    return matches;
}
