#include "../../include/types.h"

extern s32 g_main_8011F6EC_unk; /* guess: unknown word, forwarded as func_8002AC88's a3 */

extern void func_8002AC88(s32 a0, s32 a1, s16 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);

/* Dispatches event type 0x10 (kind == 0x72) or 0x11 (kind == 0x73) via
   func_8002AC88, category 5, id = val. No-op for any other kind. */
void f_main_800413AC_DispatchEvent(s16 val, s32 kind) {
    s32 type;

    if (kind == 0x72) {
        type = 0x10;
    } else if (kind == 0x73) {
        type = 0x11;
    } else {
        return;
    }

    func_8002AC88(type, 5, val, g_main_8011F6EC_unk, 0, 0, 0, 0);
}
