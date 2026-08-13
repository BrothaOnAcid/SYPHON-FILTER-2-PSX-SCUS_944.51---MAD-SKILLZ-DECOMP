#include "../../include/types.h"

extern s16 D_8011F4E8;   /* not yet decompiled: clamped to a minimum of 0x64 here */
extern s32 D_8013BF10;   /* not yet decompiled */
extern void *D_8011F32C; /* not yet decompiled: 0x80-byte scratch buffer, aliases D_8011F9E8 */
extern void *D_8011F9E8; /* not yet decompiled: same allocation as D_8011F32C */
extern s16 D_8012B02C;   /* not yet decompiled: current level id */
extern s32 D_8010C874[]; /* not yet decompiled: level name table */
extern s32 D_8011F328;   /* not yet decompiled: file handle set by func_80026234 */
extern u8 D_8013BC94[];  /* not yet decompiled: array of structs, stride 0x28 */
extern void *D_8011F9F8[]; /* not yet decompiled: per-slot 0x4000-byte buffer table */
extern u8 D_8013BF04[];  /* not yet decompiled: fixed data passed to f_init_80166728_LinkNpcSpawnRecords */

extern void *func_80025AD0(s32 size);                          /* not yet decompiled: bump allocator */
extern void func_800F045C(void *dst, char *fmt, s32 a);        /* not yet decompiled: sprintf */
extern void func_80026234(char *path, s32 *outHandle);          /* not yet decompiled: open file */
extern void func_8002723C(s32 handle, void *buf);                /* not yet decompiled: read into buf */
extern void func_8002662C(s32 *handleAddr, void *buf);           /* not yet decompiled: close/finish read */
extern void func_80010750(void *dst, void *src, s32 count);      /* not yet decompiled */
extern void func_800A3F38(char *path, void *table);              /* not yet decompiled */
extern void func_80025B3C(void *ptr);                             /* not yet decompiled: free */
extern void f_init_80166728_LinkNpcSpawnRecords(void *def, void *ctx, s32 n);
extern s32 func_80026D04(void *table);                            /* not yet decompiled: returns entry count */
extern void func_80027038(void *table, s32 index, void *out);    /* not yet decompiled */

/* guess: second half of the block splat merged into "func_80166D8C, 0x25C"
   (see knowledge.txt / f_init_80166D8C_ResetEntitySpawnState) - the real
   prologue for this function starts at 0x80166E54. Clamps D_8011F4E8 to a
   minimum of 0x64, then loads "\<level>\NPC.HOG;1" (D_8010C874[levelId])
   via func_80026234/func_8002723C/func_8002662C into a heap-allocated
   0x800-byte scratch buffer, hands it (plus the 0x80-byte D_8011F32C
   buffer) to func_80010750 and func_800A3F38 to build/parse some table,
   then frees the scratch buffer. For each of `count` slots, allocates a
   0x4000-byte
   buffer into D_8011F9F8[i] and calls f_init_80166728_LinkNpcSpawnRecords on the matching
   0x28-byte struct in D_8013BC94[]. Finishes by iterating
   func_80026D04(&D_8011F9E8)'s reported entry count through
   func_80027038 into a stack buffer (looks like some kind of enumerate
   + discard/prime pass). */
void f_init_80166E54_LoadNpcHogAndAllocSlots(s32 count) {
    char path[0x28];
    u8 out[0x18];
    void *buf;
    s32 i;
    s32 n;

    if (D_8011F4E8 < 0x64) {
        D_8011F4E8 = 0x64;
    }

    D_8013BF10 = 0;
    D_8011F32C = func_80025AD0(0x80);
    D_8011F9E8 = D_8011F32C;
    buf = func_80025AD0(0x800);

    func_800F045C(path, "\\%s\\NPC.HOG;1", D_8010C874[D_8012B02C]);
    func_80026234(path, &D_8011F328);
    func_8002723C(D_8011F328, buf);
    func_8002662C(&D_8011F328, buf);

    func_80010750(D_8011F32C, buf, 0x20);
    func_800A3F38(path, D_8011F32C);
    func_80025B3C(buf);

    for (i = 0; i < count; i++) {
        D_8011F9F8[i] = func_80025AD0(0x4000);
        f_init_80166728_LinkNpcSpawnRecords(D_8013BC94 + i * 0x28, D_8013BF04, 5);
    }

    n = func_80026D04(&D_8011F9E8);
    for (i = 0; i < n; i++) {
        func_80027038(&D_8011F9E8, i, out);
    }
}
