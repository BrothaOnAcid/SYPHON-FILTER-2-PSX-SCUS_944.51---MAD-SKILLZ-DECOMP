#include "../../include/types.h"

extern u32 D_8011ED20;

/* Guessed: builds a submission from a single global template word
   (D_8011ED20) copied onto the stack, then hands it off to func_800139C8
   (which itself calls f_main_80013000_AddPrim, so this is on the GPU
   packet-submission path) along with a fixed "1" and the caller's `count`.
   First arg is unused - dead in this function's body. */
extern void func_800139C8(u32 *word, s32 flag, s32 count);

void f_main_80013B4C_SubmitTemplateWord(void *unused, s32 count) {
    u32 w = D_8011ED20;

    func_800139C8(&w, 1, count);
}
