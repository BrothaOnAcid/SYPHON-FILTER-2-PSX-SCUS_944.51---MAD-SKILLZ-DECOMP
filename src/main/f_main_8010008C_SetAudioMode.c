#include "../../include/types.h"

/* Not in asm/800.s: 0x8010008C falls inside the misclassified data region
   (see DOC.md "Known .text/.data boundary bug"), hand-decoded from raw
   .word bytes in asm/B3BFC.s with toolz/decode_mips.py. This whole blocked
   range (0x800EFD3C-0x80168298) uses its own gp base 0x8012EC64, not
   main's 0x8011EC64 - see f_main_800FC814_DestroyMmid.c. */

/* guess: per-mode buffer table, indexed by mode id; values subtracted from
   0x80080000 to get a free-byte count passed to func_80105434/80105550 */
extern u32 D_8011D0AC[];

extern AudioSeqState g_main_8012F41C_AudioSeq;

extern void func_80105434(s32 freeBytes);
extern s32 func_80105550(s32 freeBytes);
extern SeqNode *func_80105048(s32 a0, s32 a1, s32 channel);
extern void f_main_80104F28_UnlinkSeq(SeqNode *node);
extern void func_80105134(void);
extern void f_main_80104D3C_ApplyVolume(s16 volL, s16 volR);
extern void func_80106874(s32 mode);
extern void func_80106604(s32 mode);
extern void func_801067A4(s32 active);

/* guess: switches the active audio sequencer mode to `mode`. Releases the
   buffer region of the currently active mode (if any), then tries to claim
   the buffer for the new mode; on failure re-releases the old mode's region
   and bails with -15. On success, unlinks channels 10/11 from the schedule,
   resets volume, and kicks off playback bookkeeping for the new mode. */
s32 f_main_8010008C_SetAudioMode(s32 mode) {
    s32 ret;

    if (g_main_8012F41C_AudioSeq.activeMode != 0) {
        func_80105434(0x80080000 - D_8011D0AC[g_main_8012F41C_AudioSeq.activeMode]);
    }

    if (mode != 0) {
        ret = func_80105550(0x80080000 - D_8011D0AC[mode]);
        if (ret == 0) {
            if (g_main_8012F41C_AudioSeq.activeMode != 0) {
                func_80105550(0x80080000 - D_8011D0AC[g_main_8012F41C_AudioSeq.activeMode]);
            }
            return -15;
        }
    }

    g_main_8012F41C_AudioSeq.volFlag = 1;

    f_main_80104F28_UnlinkSeq(func_80105048(0, 1, 10));
    f_main_80104F28_UnlinkSeq(func_80105048(0, 1, 11));
    func_80105134();
    f_main_80104D3C_ApplyVolume(0, 0);
    func_80106874(mode);

    if (mode != 0) {
        func_80106604(mode);
        func_801067A4(1);
    } else {
        func_80106604(g_main_8012F41C_AudioSeq.activeMode);
        func_801067A4(0);
    }

    g_main_8012F41C_AudioSeq.activeMode = mode;
    return 0;
}
