#include "../../include/types.h"

/* --- not yet decompiled --- */
extern void func_8001084C(void *tag);
extern void func_8001D358(void *tag);
extern void func_8001DA48(void *dst, s32 flags, s32 param, SoundSrcEntry *entry);
extern void *func_80016CAC(SoundListener *self, void *outPos);
extern void func_8001029C(void *arg0);
extern s32 func_80020714(SoundSrcInfo *info);
extern void func_80025004(void *space, void *tag, s16 *outVec3);
extern s32 func_8001AA5C(s32 arg0, s32 arg1);

extern s32 f_main_8001C7F8_DecodeParam(s32 packed);
extern void f_main_8001C758_ApplyReverbToActiveSlots(s32 category);

extern s32 g_main_8011EC80_unk;
extern s32 g_main_8011EC84_unk;
extern s32 g_main_8011EC78_unk;
extern s32 g_main_8011EC7C_unk;
extern s32 g_main_8011F660_SlotCount;
extern void *g_main_8011ECBC_unk;
extern u8 g_main_8011F504_unk;
extern s16 g_main_8012B02C_LevelId;
extern u32 g_main_8011EDB4_unk;
extern s32 g_main_8011EC9C_unk;
extern u8 g_main_8010C3B4_DefaultTag[];
extern void *g_main_8011EC90_unk;
extern SoundParamSlot g_main_80120514_SoundParams[];

extern PanPair g_main_801205E8_PanTarget;
extern s16 g_main_801205EC_unk;
extern PanPair g_main_80120614_ReverbTarget;
extern s16 g_main_80120618_unk;
extern s32 g_main_8011ECF4_unk;
extern void *g_main_801205DC_unk;
extern void *g_main_80120608_unk;

/* Walks the active sound-source list `sources`, recomputing per-source
   distance/pan/reverb parameters relative to listener `self` and category
   `category`'s slot in g_main_80120514_SoundParams, then committing each via
   func_8001DA48. Also refreshes the shared "current 3D pan target" globals
   (g_main_801205E8_PanTarget etc) for sources whose tag differs from the
   previous one, and re-applies reverb to already-active slots via
   f_main_8001C758_ApplyReverbToActiveSlots when the tag changes.
   Heavily unconfirmed - many globals/struct fields are best-effort guesses;
   see SoundListener/SoundSrcEntry/SoundSrcInfo in types.h. */
void f_main_8001C828_ProcessSoundSources(SoundListener *self, SoundSrcNode *sources, s32 category) {
    ListenerCore *core;
    void *sub1A4;
    u8 initFlag;
    s32 savedA, savedB;
    s32 distParam;
    s32 catParam;
    s32 subFlagF0;
    s32 rangeThreshold;
    PanPair bkPan;
    s16 bkPanHi;
    PanPair bkReverb;
    s16 bkReverbHi;
    s32 bkMode;
    s32 i;
    s32 pos[3];
    s32 distAbs;
    s32 subFlagE8;
    s32 collected[0x20];
    s32 collectedCount;

    initFlag = 1;
    savedA = 0;
    savedB = 0;
    collectedCount = 0;
    distParam = g_main_8011EC80_unk;
    core = self->core;
    sub1A4 = self->sub1A4;
    catParam = g_main_80120514_SoundParams[category].val;
    if (sub1A4 != 0) {
        subFlagF0 = ((SoundSrcEntry *) sub1A4)->info->flags28 & 0x20000;
    } else {
        subFlagF0 = 0;
    }
    rangeThreshold = 0x960;
    bkPan = g_main_801205E8_PanTarget;
    bkPanHi = g_main_801205EC_unk;
    bkReverb = g_main_80120614_ReverbTarget;
    bkReverbHi = g_main_80120618_unk;
    bkMode = g_main_8011ECF4_unk;
    if ((u16) self->unk4 >= 0x2B) {
        rangeThreshold = 0x640;
    }

    if (g_main_8011F660_SlotCount > 0) {
        for (i = 0; i < g_main_8011F660_SlotCount; i++) {
            self->slots[i] = 0;
        }
    }
    g_main_8011ECBC_unk = self->slots;

    savedB = f_main_8001C7F8_DecodeParam(g_main_8011EC84_unk);
    savedB = (savedB >> 2) + savedB;
    g_main_8011EC7C_unk = savedB;
    if (distParam != 0) {
        savedA = f_main_8001C7F8_DecodeParam(distParam);
    }

    *(s32 *) 0x1F8003FC = core->posY + (core->posZ << 0x10);

    core = self->core;
    pos[0] = core->posX;
    pos[1] = -core->posY;
    pos[2] = core->posZ;
    {
        void *hit = func_80016CAC(self, pos);
        if (hit != 0) {
            subFlagE8 = ((SoundSrcEntry *) hit)->info->flags28 & 0x20000;
        } else {
            subFlagE8 = 0x29A;
        }
    }

    if ((self->unk9 != 0) || (self->unk9C != 0 && *(s32 *) self->unk9C == 0) || (self->unk6 & 0x10)) {
        g_main_8011F504_unk = 0;
    } else {
        g_main_8011F504_unk = 1;
    }

    core = self->core;
    {
        s32 dz = -(s32) core->rotZ;

        distAbs = dz < 0 ? -dz : dz;
    }

    if (g_main_8012B02C_LevelId == 0) {
        if (distAbs < 0x44D) {
            rangeThreshold = 0x180;
            if (distAbs >= 0x385) {
                rangeThreshold = 0x2BC;
            }
        } else {
            rangeThreshold = 0x400;
        }
    } else if (distAbs < 0x579) {
        if (distAbs < 0x44D) {
            rangeThreshold = 0x100;
            if (distAbs >= 0x385) {
                rangeThreshold = 0x180;
            }
        } else {
            rangeThreshold = 0x200;
        }
    } else {
        rangeThreshold = 0x400;
    }

    if (self->unk9 == 0) {
        g_main_8011EDB4_unk += 1;
        if (g_main_8011EC9C_unk > 0) {
            g_main_8011EC9C_unk -= 1;
        }
    }

    if (sources != 0) {
        SoundSrcNode *node = sources;

        do {
            SoundSrcEntry *entry = node->entry;
            SoundSrcInfo *info = entry->info;
            s32 overrideFlag = 0;

            if ((info->flags28 & 0x01000000) && !(entry->flags8 & 8)) {
                s32 reverbFlags = rangeThreshold;
                s32 isDefaultTag = entry->unkC == g_main_8010C3B4_DefaultTag;
                s32 runFlagSwitch = 1;

                if (isDefaultTag && g_main_8011EC90_unk == 0 && initFlag == 0) {
                    runFlagSwitch = g_main_8011EC9C_unk > 0;
                }
                if (runFlagSwitch) {
                    if (initFlag != 0) {
                        func_8001084C(g_main_8010C3B4_DefaultTag);
                        f_main_8001C758_ApplyReverbToActiveSlots(category);
                        initFlag = 0;
                    }
                    func_8001D358(entry->unkC);
                    if (g_main_8011EC90_unk != 0) {
                        reverbFlags |= 0x80000000;
                    }
                    if (g_main_8011EC9C_unk > 0) {
                        reverbFlags |= 0x40000000;
                    }
                }

                if (self->unk6 & 0x10) {
                    reverbFlags = 0x200000C8;
                }
                if (info->flags28 & 0x200000) {
                    overrideFlag = 1;
                    g_main_8011EC78_unk = 0x3FFF;
                    g_main_8011EC7C_unk = 0x2000;
                }

                if (!isDefaultTag) {
                    reverbFlags = (reverbFlags & 0xFFFF0000) | ((u16) (reverbFlags & 0xFFFF) * 2);
                    if (info->flags28 & 0x40000) {
                        g_main_80120514_SoundParams[category].val = catParam + 0x100;
                        bkMode = 0x40;
                    }
                    if (distParam != 0) {
                        overrideFlag = 1;
                        g_main_8011EC78_unk = distParam;
                        g_main_8011EC7C_unk = savedA;
                    }
                    if ((entry->flagsA & 0x20) || (func_8001AA5C(0x1F40, 0xC8) != 0)) {
                        if (entry->flagsA & 0x40) {
                            g_main_80120514_SoundParams[category].val = catParam + 0x48;
                            bkMode = 0x12;
                        }
                        if (entry->flagsB & 0x40) {
                            s16 vec[3];

                            func_80025004(g_main_801205DC_unk, entry->unkC, vec);
                            g_main_801205E8_PanTarget.lo = vec[0];
                            g_main_801205EC_unk = vec[1];
                            g_main_801205E8_PanTarget.hi = -vec[2];
                            func_80025004(g_main_80120608_unk, entry->unkC, vec);
                            g_main_8011ECF4_unk = 2;
                            g_main_80120614_ReverbTarget.lo = vec[0];
                            g_main_80120614_ReverbTarget.hi = -vec[2];
                            g_main_80120618_unk = vec[1];
                        }
                        goto commit;
                    }
                } else {
                    u32 flags2 = info->flags28;

                    if ((flags2 & 0x40000) ||
                        ((subFlagE8 == subFlagF0) && (subFlagE8 != 0x29A) && (subFlagE8 != (s32) (flags2 & 0x20000)))) {
                        g_main_80120514_SoundParams[category].val = catParam + 0x48;
                        bkMode = 0x12;
                    }
                }

            commit:
                func_8001D358(entry->unkC);
                if (collectedCount < 0x1F) {
                    collected[collectedCount] = (s32) PTR_U32(info->unk20);
                    collectedCount += 1;
                }
                {
                    s32 t2 = func_80020714(info);
                    TargetInfo *tgt = (entry->flagsA & 4) ? (TargetInfo *) info->unk24 : (TargetInfo *) info->unk20;

                    if (isDefaultTag && g_main_8011EC90_unk != 0) {
                        FlagListNode *ln = g_main_8011EC90_unk;

                        do {
                            FlagNode *item = ln->item;
                            u32 f = item->flags4;
                            u32 fClear = f & ~4;

                            item->flags4 = fClear;
                            if (f & 2) {
                                u8 *bitmapBase = (u8 *) item + 0x40;
                                s32 align = (s32) PTR_U32(bitmapBase) & 3;
                                s32 bitIdx = ((TargetInfo *) info->unk20)->unk2;

                                if (align != 0) {
                                    bitIdx += align * 8;
                                    bitmapBase = (u8 *) (u64) (PTR_U32(bitmapBase) & ~3);
                                }
                                if (!(((u32 *) bitmapBase)[bitIdx >> 5] & (1u << (bitIdx & 0x1F)))) {
                                    item->flags4 = fClear | 4;
                                }
                            }
                            ln = ln->next;
                        } while (ln != 0);
                    }

                    {
                        s32 v0_2 = isDefaultTag;
                        s32 v0_11;

                        if (((tgt->unk2 != 0xF0) || (v0_2 = isDefaultTag, entry->unk18 == self)) &&
                            (((v0_11 = t2 & 0xFF) != 0) || (v0_2 != 0))) {
                            s32 a2 = 0;

                            if ((v0_11 != 0) || (rangeThreshold < distAbs)) {
                                a2 = g_main_80120514_SoundParams[category].val;
                            }
                            func_8001DA48(tgt, reverbFlags, a2, entry);
                        }
                    }
                }

                g_main_80120514_SoundParams[category].val = catParam;
                {
                    s32 v0_3 = isDefaultTag;

                    if (overrideFlag) {
                        g_main_8011EC7C_unk = savedB;
                        g_main_8011EC78_unk = g_main_8011EC84_unk;
                        v0_3 = isDefaultTag;
                    }
                    if (v0_3 == 0) {
                        func_8001D358(g_main_8010C3B4_DefaultTag);
                        if (entry->flagsB & 0x40) {
                            g_main_801205E8_PanTarget = bkPan;
                            g_main_801205EC_unk = bkPanHi;
                            g_main_80120614_ReverbTarget = bkReverb;
                            g_main_80120618_unk = bkReverbHi;
                            g_main_8011ECF4_unk = bkMode;
                        }
                    }
                }
            }

            node = node->next;
        } while (node != 0);
    }

    if ((self->unk9 == 0) && (g_main_8011EDB4_unk & 1)) {
        for (i = 0; i < collectedCount; i++) {
            func_8001029C((void *) (u64) (u32) collected[i]);
        }
    }
}
