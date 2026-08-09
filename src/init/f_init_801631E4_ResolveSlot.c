#include "../../include/types.h"

extern s32 f_init_801630EC_ResolveSlot(ResourceSlot *slot, u8 requireCount, u8 initExtra);

/* Thin wrapper: f_init_801630EC_ResolveSlot with initExtra forced off. */
s32 f_init_801631E4_ResolveSlot(ResourceSlot *slot, u8 requireCount) {
    return f_init_801630EC_ResolveSlot(slot, requireCount, 0);
}
