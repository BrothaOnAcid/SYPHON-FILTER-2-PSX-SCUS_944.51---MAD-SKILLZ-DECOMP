#include "../../include/types.h"

/* Guessed object with a mode/state word at +0x64. */
typedef struct {
    u8 _pad00[0x64];
    u32 mode;   /* +0x64 */
} ModeObj;

/* Guessed 0x18-byte GPU-packet-like record (D_80169728/D_8016972C are the
   splat-assigned names for this struct's first two fields; the remaining
   0x10 bytes are untouched by this function). Array of 5, all BSS/zero. */
typedef struct {
    u32 tag0;      /* +0x00 (D_80169728) */
    u32 tag1;      /* +0x04 (D_8016972C) */
    u8 _pad08[0x10];
} CmdRecord;

extern ModeObj *g_init_80114704_CurObj;
extern CmdRecord g_init_80169728_CmdRecords[5];

/* Only runs when g_init_80114704_CurObj->mode == 8. Builds a tag/code pair
   into each of 5 command records; the (i+0x4F) index and 0xFF82 constant
   suggest GPU/OT-style command headers, but the exact format is
   unconfirmed. */
void f_init_8015CA60_BuildCommandTable(s32 base) {
    s32 i;
    u16 lo;

    if (g_init_80114704_CurObj->mode != 8)
        return;

    lo = (u16) (base - 0x7E);

    for (i = 0; i < 5; i++) {
        u32 tag = (u32) (i + 0x4F) << 16;

        g_init_80169728_CmdRecords[i].tag0 = tag | 0xFF82;
        g_init_80169728_CmdRecords[i].tag1 = tag | lo;
    }
}
