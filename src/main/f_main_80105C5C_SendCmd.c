#include "../../include/types.h"

/* Not in asm/800.s: 0x80105C5C falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c. */

/* PSYQ primitive - the actual low-level engine/hardware command
   transmitter (720 bytes); 0xCC/0xCD look like fixed protocol op-codes.
   Left extern per "except psyq calls". */
extern void func_80107E8C(s32 cmd, u32 arg, s32 op1, s32 op2);

/* thin wrapper: forwards (cmd, arg) to the low-level command transmitter
   with fixed op-codes 0xCC/0xCD. */
void f_main_80105C5C_SendCmd(s32 cmd, u32 arg) {
    func_80107E8C(cmd, arg, 0xCC, 0xCD);
}
