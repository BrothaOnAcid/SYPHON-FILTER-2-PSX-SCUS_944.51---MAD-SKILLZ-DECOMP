#include "../../include/types.h"

extern void (*g_main_8011F2E8_DespawnCallback)(s16 id);

extern void func_8002AC88(s32 a0, s32 a1, s16 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
extern void func_8002E0D4(void *obj, s32 arg1, s32 arg2);

/* arg0->unk2: this object's id. If a global despawn callback is set, it's
   invoked first with the id; then a type-1/mode-2 event is dispatched
   (id | 0xA0000 encoded in a3), and the object itself is torn down via
   func_8002E0D4. */
void f_main_800AB8E8_DespawnAndNotify(void *arg0, s32 arg1) {
    s16 id = *(s16 *) ((u8 *) arg0 + 0x2);

    if (g_main_8011F2E8_DespawnCallback != 0)
        g_main_8011F2E8_DespawnCallback(id);

    func_8002AC88(1, 2, -1, 0xFFFF, (u16) id | 0xA0000, 0, 0, 0);
    func_8002E0D4(arg0, arg1, 0);
}
