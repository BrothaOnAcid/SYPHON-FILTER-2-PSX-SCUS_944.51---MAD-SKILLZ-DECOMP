#include "../../include/types.h"

/* The raw PS1 GTE ops below (ctc2) cannot be compiled on a non-MIPS host, so
   they are modeled as this placeholder macro; the real write goes to the GTE
   control register file at runtime. */
#define GTE_CTC2(v, reg) ((void) (v))

/* Writes the GTE projection control registers via ctc2: OFY (reg 13), H
   (screen height, reg 14) and DQA (reg 15). Callers scale their byte values
   by 16 before calling. */
void f_main_8001AAD4_SetGteProj(u32 ofy, u32 h, u32 dqa) {
    GTE_CTC2(ofy, 13);   /* OFY */
    GTE_CTC2(h, 14);     /* H   */
    GTE_CTC2(dqa, 15);   /* DQA */
}
