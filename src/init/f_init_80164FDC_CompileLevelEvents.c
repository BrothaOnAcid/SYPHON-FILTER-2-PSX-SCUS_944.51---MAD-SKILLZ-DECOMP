#include "../../include/types.h"

extern s16 D_8011F330;   /* not yet decompiled */
extern s16 D_8011F332;   /* not yet decompiled */
extern s16 D_8011F312;   /* not yet decompiled */
extern s32 D_8011F304;   /* not yet decompiled */
extern s32 D_8011FA1C;   /* not yet decompiled */
extern s32 D_8011F334;   /* not yet decompiled */
extern s32 D_8011F314;   /* not yet decompiled */
extern s32 D_8011F318;   /* not yet decompiled */
extern s32 D_8011F340;   /* not yet decompiled */
extern s32 D_8013BEE4;   /* not yet decompiled */
extern u8 D_80168940;    /* not yet decompiled: one-time init guard */
extern s16 D_8012B02C;   /* not yet decompiled: current level id */
extern s32 D_8010C874[]; /* not yet decompiled: level name table */
extern void *D_8011F9D4; /* not yet decompiled: loaded level-event buffer */
extern s16 D_8011FA14;   /* not yet decompiled: entry count, mirrors the buffer's header word */
extern void *D_8013C030[]; /* not yet decompiled: entry-pointer lookup table */
extern s32 D_8011FA04;   /* not yet decompiled: "LEVEL" handle */

extern void func_800B2A64(void);                    /* not yet decompiled: one-time subsystem init */
extern void func_800F045C(void *dst, char *fmt, s32 a, s32 b); /* not yet decompiled: sprintf */
extern u32 f_main_8002B0D0_LoadFileAlloc(s8 *name, void **outPtr, s32 size);
extern void func_800B3D34(void);                    /* not yet decompiled */
extern s32 f_main_800B3920_FindLevelEventByName(char *name);

/* guess: loads and "compiles" a level's event script (\<dir>\<name>.S;1,
   built from D_8010C874[levelId]). Resets a handful of related globals to
   their empty state first (one-time func_800B2A64 init gated by
   D_80168940). If the load succeeds, the buffer starts with a s16 entry
   count followed by an array of s32 self-relative offsets; each entry
   pointed to by `base + offset[i]` has 6 s32 fields (+0x8, +0xC, +0x10,
   +0x14, +0x18, +0x1C) that are likewise self-relative and get fixed up
   in place into absolute pointers (entry + field), with the fixed-up
   entry pointer recorded into D_8013C030[i]. Finishes by either resolving
   the "LEVEL" handle via f_main_800B3920_FindLevelEventByName (if `flag`
   is set) or running func_800B3D34 instead. */
void f_init_80164FDC_CompileLevelEvents(s32 flag) {
    char path[0x38];

    D_8011F330 = -1;
    D_8011F332 = -1;
    D_8011F312 = -1;
    D_8011F304 = 0;
    D_8011FA1C = 0;
    D_8011F334 = 0;
    D_8011F314 = 0;
    D_8011F318 = 0;
    D_8011F340 = -1;
    D_8013BEE4 = 0;

    if (D_80168940 == 0) {
        func_800B2A64();
        D_80168940 = 1;
    }

    func_800F045C(path, "\\%s\\%s.S;1", D_8010C874[D_8012B02C], D_8010C874[D_8012B02C]);

    if (f_main_8002B0D0_LoadFileAlloc((s8 *) path, &D_8011F9D4, 0) != 0) {
        u8 *b = (u8 *) D_8011F9D4;
        s32 n = *(s16 *) b;
        s32 *off = (s32 *) (b + 4);
        static const s32 kFixup[6] = { 0x8, 0xC, 0x10, 0x14, 0x18, 0x1C };
        s32 i;

        D_8011FA14 = *(s16 *) b;

        for (i = 0; i < n; i++) {
            u8 *e = b + off[i];
            s32 j;

            for (j = 0; j < 6; j++) {
                s32 *f = (s32 *) (e + kFixup[j]);
                *f = (s32) (e + *f);
            }

            D_8013C030[i] = e;
        }

        if (flag != 0) {
            D_8011FA04 = f_main_800B3920_FindLevelEventByName("LEVEL");
        } else {
            func_800B3D34();
        }
    }
}
