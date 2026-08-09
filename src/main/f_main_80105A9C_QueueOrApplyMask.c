#include "../../include/types.h"

/* Not in asm/800.s: 0x80105A9C falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c.

   Per knowledge.txt: takes TWO args (cmd, mask), not one as originally
   guessed elsewhere. cmd 1 = enable channels in `mask`, cmd 0 = disable;
   any other cmd is a no-op. If the engine is mid-tick
   (g_main_8011E440_EngineBusy bit 0), the command is queued into a 4-slot
   pending-command struct (g_main_8014210C_PendingCmd) with opposing
   pending bits cross-cancelled; otherwise it's applied immediately to
   g_main_8011E3E0_ChannelMask and mirrored into the mixer object. */

extern void *g_main_8011E374_MixerPtr;   /* see f_main_80104D3C_ApplyVolume */
extern u32 g_main_8011E3E0_ChannelMask;
extern u32 g_main_8011E408_PendingEnableMask;
extern u32 g_main_8011E40C_PendingFlag;
extern u32 g_main_8011E440_EngineBusy;

typedef struct {
    u16 enableLo;   /* +0x0 */
    u16 enableHi;   /* +0x2 */
    u16 disableLo;  /* +0x4 */
    u16 disableHi;  /* +0x6 */
} PendingCmd;

extern PendingCmd g_main_8014210C_PendingCmd;

void f_main_80105A9C_QueueOrApplyMask(s32 cmd, u32 arg) {
    u32 mask = arg & 0xFFFFFF;
    u32 hiMask = mask >> 16;

    if (cmd == 1) {
        if ((g_main_8011E440_EngineBusy & 1) == 0) {
            g_main_8011E3E0_ChannelMask |= mask;
            *(u16 *) ((u8 *) g_main_8011E374_MixerPtr + 0x188) = (u16) mask;
            *(u16 *) ((u8 *) g_main_8011E374_MixerPtr + 0x18A) = (u16) hiMask;
        } else {
            g_main_8014210C_PendingCmd.enableLo = (u16) mask;
            g_main_8014210C_PendingCmd.enableHi = (u16) hiMask;
            g_main_8011E40C_PendingFlag |= 1;
            g_main_8011E408_PendingEnableMask |= mask;

            if (g_main_8014210C_PendingCmd.disableLo & mask) {
                g_main_8014210C_PendingCmd.disableLo &= ~mask;
            }
            if (g_main_8014210C_PendingCmd.disableHi & hiMask) {
                g_main_8014210C_PendingCmd.disableHi &= ~hiMask;
            }
        }
    } else if (cmd == 0) {
        if ((g_main_8011E440_EngineBusy & 1) == 0) {
            *(u16 *) ((u8 *) g_main_8011E374_MixerPtr + 0x18C) = (u16) mask;
            *(u16 *) ((u8 *) g_main_8011E374_MixerPtr + 0x18E) = (u16) hiMask;
            g_main_8011E3E0_ChannelMask &= ~mask;
        } else {
            g_main_8014210C_PendingCmd.disableLo = (u16) mask;
            g_main_8014210C_PendingCmd.disableHi = (u16) hiMask;
            g_main_8011E40C_PendingFlag |= 1;
            g_main_8011E408_PendingEnableMask &= ~mask;

            if (g_main_8014210C_PendingCmd.enableLo & mask) {
                g_main_8014210C_PendingCmd.enableLo &= ~mask;
            }
            if (g_main_8014210C_PendingCmd.enableHi & hiMask) {
                g_main_8014210C_PendingCmd.enableHi &= ~hiMask;
            }
        }
    }
}
