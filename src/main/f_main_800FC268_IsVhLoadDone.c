#include "../../include/types.h"

/* Blocked-region code (0x800EFD3C-0x80168298, see DOC.md), hand-decoded
   from raw words with toolz/decode_mips.py; uses the alternate gp base
   0x8012EC64. Polled by f_main_800FC13C_BeginVhLoad's caller
   (f_init_80168190_LoadSongTree) until it returns nonzero. */
extern s32 g_main_8012F400_LoadBusy;

s32 f_main_800FC268_IsVhLoadDone(void) {
    return g_main_8012F400_LoadBusy == 0;
}
