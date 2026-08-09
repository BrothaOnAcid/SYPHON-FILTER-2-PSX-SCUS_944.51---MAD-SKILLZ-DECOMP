#include "../../include/types.h"

/* Not in asm/800.s: 0x800F616C falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c. */

/* PSYQ primitive - writes a0[0..3] out through 3 fixed register-pointer
   slots (0x8011D480/4/8/C) with interleaved command bytes 2/3/0x20; a raw
   hardware/engine command-port sequence, not game logic. Left extern. */
extern s32 func_80103D74(u8 *buf4);

/* thin wrapper: forwards a 4-byte config buffer to the low-level engine
   command writer, always returns 1. */
s32 f_main_800F616C_SendEngineConfig(u8 *buf4) {
    func_80103D74(buf4);
    return 1;
}
