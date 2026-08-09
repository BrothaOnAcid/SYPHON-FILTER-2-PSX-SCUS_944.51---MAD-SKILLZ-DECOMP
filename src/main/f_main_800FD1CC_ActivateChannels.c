#include "../../include/types.h"

/* PSYQ audio-sequencer state (library's own gp base 0x8012EC64). */
extern AudioSeqState g_main_8012F41C_AudioSeq;

/* Main-side "music" mirror of the volL/volR/volFlag fields of
   AudioSeqState (accessed here via absolute lui/lw, not $gp - these sit in
   main's own gp-relative region 0x8011EC64, inside the music-sequencer
   state block 0x8011F370..0x8011F49C noted in knowledge.txt). Offsets from
   0x8011F41C exactly match AudioSeqState's +0x40/+0x42/+0x44, suggesting
   the two structs share layout. */
extern s16 g_main_8011F45C_VolL;
extern s16 g_main_8011F45E_VolR;
extern u32 g_main_8011F460_VolFlag;

/* --- libc-style externs (not yet decompiled) --- */
extern void f_main_80105C5C_SendCmd(s32 cmd, u32 arg);
extern void f_main_80105A9C_QueueOrApplyMask(s32 cmd, u32 arg);
                                                  f_main_800FD258_ReapPending
                                                  (called there as cmd 0) */
extern void f_main_80104D3C_ApplyVolume(s16 volL, s16 volR);

/* Companion of f_main_800FD258_ReapPending (both are called from the
   audio-poll dispatch at 0x80104CF0). When `mask` (newly in-use channels)
   is non-zero: notifies the engine (cmd 8 with flagsC, cmd 1 with mask),
   promotes those channels from flagsB into flagsA, clears mask, and - if
   the main-side volume mirror is dirty - applies the pending L/R volume
   and clears the dirty flag. */
void f_main_800FD1CC_ActivateChannels(void) {
    u32 mask = g_main_8012F41C_AudioSeq.mask;

    if (mask != 0) {
        f_main_80105C5C_SendCmd(8, g_main_8012F41C_AudioSeq.flagsC);
        f_main_80105A9C_QueueOrApplyMask(1, g_main_8012F41C_AudioSeq.mask);

        mask = g_main_8012F41C_AudioSeq.mask;
        g_main_8012F41C_AudioSeq.mask = 0;
        g_main_8012F41C_AudioSeq.flagsB &= ~mask;
        g_main_8012F41C_AudioSeq.flagsA |= mask;

        if (g_main_8011F460_VolFlag != 0) {
            f_main_80104D3C_ApplyVolume(g_main_8011F45C_VolL, g_main_8011F45E_VolR);
            g_main_8011F460_VolFlag = 0;
        }
    }
}
