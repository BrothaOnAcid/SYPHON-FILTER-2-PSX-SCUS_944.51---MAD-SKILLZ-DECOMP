#include "../../include/types.h"

extern u8 g_main_8011F624_Flag;   /* "isLowLevel"/freeze flag, see f_main_8002A338_LoadLevel */
extern s32 g_main_8011F1AC_unk;   /* persisted danger accumulator, reset in f_init_80168298_InitSoundSystem */
extern u8 g_main_8011F1A7_unk;    /* extra "add bonus" gate, also reset there */
extern void **g_main_8011EEFC_ObjArray;

extern s8 g_main_801150C8_DistWeight[5]; /* guess: per-candidate distance weight table */
extern s8 g_main_801150CC_DistDivisor;   /* guess */
extern s8 g_main_801150CD_BaseDanger0;   /* guess: base danger for priority-band 0 */
extern s8 g_main_801150CE_BaseDanger1;   /* guess: base danger for priority-band 1 */
extern s8 g_main_801150CF_BaseDanger2;   /* guess: base danger for priority-band [0x547,0xAB8) */
extern s8 g_main_801150D0_BaseDanger3;   /* guess: base danger for priority-band [0xAB8,0x1000) */
extern s8 g_main_801150D1_BaseDanger4;   /* guess: base danger for priority-band ==0x1000 */
extern s8 g_main_801150D2_Bonus;         /* guess: extra bonus when g_main_8011F1A7_unk is set */

/* enemy iterator: mode 0 = start a new search, -1 = get next; category is
   always 0xB here. Returns a g_main_8011EEFC_ObjArray index, or negative
   when the search is exhausted. */
extern s32 func_80077DA4(s32 mode, s32 category);
extern void func_800273DC(s32 *posA, s32 *posB, s32 *outDistSq);
extern void func_8008E468(s32 target); /* eases the real danger value toward `target` */

/* Reads a candidate actor's world position through its ->0x8->0xC chain
   (offsets guessed from usage only - real struct layout unknown); y is
   negated to match func_800273DC's coordinate convention. */
static void GetDangerPos(void *actor, s32 *outPos) {
    u8 *link = *(u8 **) ((u8 *) actor + 0x8);
    u8 *pos = *(u8 **) (link + 0xC);

    outPos[0] = *(s32 *) (pos + 0x14);
    outPos[1] = -*(s32 *) (pos + 0x18);
    outPos[2] = *(s32 *) (pos + 0x1C);
}

/* Per-frame "danger level" scorer for the music sequencer (matches
   knowledge.txt's g_main_8011F394 "decrementing danger var" note - this
   is what feeds it a new target each frame). If g_main_8011F624_Flag is
   set (frozen/non-scoring levels), skips straight to re-applying the
   last accumulated value g_main_8011F1AC_unk.

   Otherwise: walks nearby category-0xB entities via func_80077DA4,
   deriving each one's "priority" from its ->0x14->0xE8 word (masked,
   then forced up to 0xAB8 by a couple of status-flag checks off
   ->0x1C), tracking the highest priority seen. Entities that pass a
   count gate (->0x18->0x8 > 0) also contribute to a running-minimum
   distance-to-self, with each displaced ("runner-up") minimum recorded
   into a small local list (capped at 5).

   The final highest priority is classified into one of five bands
   (g_main_801150CD..D1 base-danger bytes), summed with a weighted
   contribution from the recorded runner-up distances
   (g_main_801150C8 weight table / g_main_801150CC divisor) and the
   persisted accumulator g_main_8011F1AC_unk (plus g_main_801150D2 if
   g_main_8011F1A7_unk is set), clamped to [1,100], and handed to
   func_8008E468.

   CAVEAT: the entity field offsets, priority mask, and banding
   constants are best-effort guesses reconstructed from an m2c first
   pass; not bit-exact-verified. Revisit if a cleaner source of truth
   turns up. */
void f_main_8008E4BC_UpdateDangerLevel(void *self) {
    s32 selfPos[3];
    s32 candPos[3];
    s32 dist[5];
    s32 minDist = 0x5340;
    s32 count = 0;
    s32 bestPriority = 0;
    s32 idx;
    s32 base;
    s32 weighted;
    s32 target;
    s32 i;

    if (g_main_8011F624_Flag != 0) {
        func_8008E468(g_main_8011F1AC_unk);
        return;
    }

    GetDangerPos(self, selfPos);

    for (idx = func_80077DA4(0, 0xB); idx >= 0; idx = func_80077DA4(-1, 0xB)) {
        void *cand = g_main_8011EEFC_ObjArray[idx];
        u8 *priorityObj = *(u8 **) ((u8 *) cand + 0x14);
        u8 *countObj = *(u8 **) ((u8 *) cand + 0x18);
        void *statusObj = *(void **) ((u8 *) cand + 0x1C);
        s32 priority = *(s32 *) (priorityObj + 0xE8) & 0xFFFF3FFF;

        GetDangerPos(cand, candPos);

        if (statusObj != 0) {
            u32 flags0 = *(u32 *) statusObj;
            u32 flags4 = *(u32 *) ((u8 *) statusObj + 4);

            if ((flags0 & 1) || (flags4 & 0x402))
                priority = 0xAB8;
        } else if (priority != 0) {
            priority = 0xAB8;
        }

        if (bestPriority < priority)
            bestPriority = priority;

        if (priority > 0 && *(s16 *) (countObj + 8) > 0) {
            s32 distSq;

            func_800273DC(selfPos, candPos, &distSq);
            if (distSq < minDist) {
                if (count < 5)
                    dist[count] = minDist;
                minDist = distSq;
            } else if (count < 5) {
                dist[count] = distSq;
            }
            if (count < 5)
                count++;
        }
    }

    if (bestPriority == 0)
        base = g_main_801150CD_BaseDanger0;
    else if (bestPriority == 1)
        base = g_main_801150CE_BaseDanger1;
    else if ((u32) (bestPriority - 0x547) < 0x571)
        base = g_main_801150CF_BaseDanger2;
    else if ((u32) (bestPriority - 0xAB8) < 0x548)
        base = g_main_801150D0_BaseDanger3;
    else if (bestPriority == 0x1000)
        base = g_main_801150D1_BaseDanger4;
    else
        base = 0;

    weighted = 0;
    for (i = 0; i < count; i++) {
        s32 falloff = ((0xC80 - dist[i]) >> 5) / g_main_801150CC_DistDivisor;
        s32 clamped = falloff > 0 ? falloff : 0;

        weighted += (g_main_801150C8_DistWeight[i] * clamped) / g_main_801150CC_DistDivisor;
    }

    target = base + weighted + g_main_8011F1AC_unk;
    if (g_main_8011F1A7_unk != 0)
        target += g_main_801150D2_Bonus;

    if (target > 100)
        target = 100;
    else if (target <= 0)
        target = 1;

    func_8008E468(target);
}
