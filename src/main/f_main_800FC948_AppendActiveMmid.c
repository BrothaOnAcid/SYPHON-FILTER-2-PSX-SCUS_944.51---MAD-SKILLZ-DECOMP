#include "../../include/types.h"

/* Not in asm/800.s: 0x800FC948 falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c. */

extern MmidObj *g_main_8012F40C_ActiveMmid;

/* appends `obj` to the tail of the global active-MMID list
   (g_main_8012F40C_ActiveMmid, via MmidObj.next), the same list
   f_main_800FC814_DestroyMmid unlinks from. */
void f_main_800FC948_AppendActiveMmid(MmidObj *obj) {
    MmidObj *node;

    if (g_main_8012F40C_ActiveMmid == 0) {
        g_main_8012F40C_ActiveMmid = obj;
        return;
    }

    node = g_main_8012F40C_ActiveMmid;
    while (node->next != 0) {
        node = node->next;
    }
    node->next = obj;
}
