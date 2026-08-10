#include "../../include/types.h"

/* guess: wraps `val` into [0, mod). Returns 1 (and leaves *out == val)
   if mod is 0; otherwise folds one step by +-mod, then falls back to a
   full modulo if that single step wasn't enough. Returns 0 on the
   normal path. */
s32 f_main_80022858_WrapValue(s32 val, s32 mod, s32 *out) {
    *out = val;
    if (mod == 0)
        return 1;

    if (val < 0)
        *out = val + mod;
    else if (val >= mod)
        *out = val - mod;

    if (*out < 0 || *out >= mod)
        *out %= mod;

    return 0;
}
