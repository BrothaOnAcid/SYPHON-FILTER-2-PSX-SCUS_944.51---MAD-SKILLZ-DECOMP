#include "../../include/types.h"

extern u8 D_80158AE8[];                         /* not yet decompiled: string literal */
extern void (*D_8016881C[])(TokenCursor *);     /* not yet decompiled: opcode -> handler table */

extern void func_8015944C(void);                                   /* not yet decompiled */
extern u32 f_main_8002B0D0_LoadFileAlloc(s8 *name, void **outPtr, s32 size);
extern void func_801594F4(TokenCursor *cur, s16 *out);              /* not yet decompiled: reads/skips one token */

/* guess: script/opcode interpreter. f_main_8002B0D0_LoadFileAlloc resolves
   D_80158AE8 into a raw byte stream, storing the cursor's read position
   directly into cur.pos; func_801594F4(&cur, &count) then reads the entry
   count. For each
   of `count` entries: skip any run of 0xFE filler bytes at the cursor, then
   read an opcode (func_801594F4 into `op`) and an argument (func_801594F4
   into the cursor itself - matches the asm, which reuses the cursor's own
   address as the output pointer for this read), then call
   D_8016881C[op](cursor). */
void f_init_8015BA54_RunTokenScript(void) {
    TokenCursor cur;
    s16 count;
    s16 op;
    s32 i;

    func_8015944C();
    f_main_8002B0D0_LoadFileAlloc((s8 *) D_80158AE8, (void **) &cur.pos, 0);
    func_801594F4(&cur, &count);
    for (i = 0; i < count; i++) {
        if (*cur.pos == 0xFE) {
            do {
                cur.pos++;
            } while (*cur.pos == 0xFE);
        }
        func_801594F4(&cur, &op);
        func_801594F4(&cur, (s16 *) &cur);
        D_8016881C[op](&cur);
    }
}
