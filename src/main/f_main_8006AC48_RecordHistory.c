#include "../../include/types.h"

extern u32 g_main_8011F728_HistoryCount;
extern HistoryRecord g_main_8012FAEC_HistoryBuf[4];

/* guess: appends a 0x3C-byte snapshot of `buf` (with its first word forced
   to `arg0`) into a 4-slot history ring that stops recording once full
   (never wraps). */
void f_main_8006AC48_RecordHistory(s32 arg0, s32 *buf) {
    if (g_main_8011F728_HistoryCount < 4) {
        HistoryRecord *dst = &g_main_8012FAEC_HistoryBuf[g_main_8011F728_HistoryCount];
        s32 i;

        buf[0] = arg0;
        for (i = 0; i < 15; i++)
            dst->words[i] = buf[i];

        g_main_8011F728_HistoryCount++;
    }
}
