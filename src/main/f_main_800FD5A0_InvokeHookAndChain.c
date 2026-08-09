#include "../../include/types.h"

/* Guessed optional callback slot, zero-initialized; called by
   f_main_800FD5A0_InvokeHookAndChain if set. Lives 8 bytes before
   AudioSeqState (g_main_8012F41C_AudioSeq) at the library's own gp base
   0x8012EC64 - NOT main's gp 0x8011EC64 (this function is in the PSYQ
   blocked region 0x800EFD3C-0x80168298; see knowledge.txt). */
extern void (*g_main_8012F414_Hook)(void);

/* --- libc-style externs (not yet decompiled) --- */
extern void f_main_800FD774_MuteIdleChannelsPre(void);
extern void f_main_800FD610_MuteIdleChannels(void);

/* Invokes the optional hook (if any), then always chains into two fixed
   follow-up routines. Called from func_8008D7A0's "flag set" path. */
void f_main_800FD5A0_InvokeHookAndChain(void) {
    if (g_main_8012F414_Hook != 0)
        g_main_8012F414_Hook();
    f_main_800FD774_MuteIdleChannelsPre();
    f_main_800FD610_MuteIdleChannels();
}
