#include "../../include/types.h"

void f_init_8016112C_UpdateChainFlags(ObjRecord *arg0, s32 cmd);
void f_init_80161E28_UpdateWeaponTrack(ObjRecord *arg0);
void f_init_8015EBB8_PropagateWeaponChain(ObjRecord *arg0);
void f_init_801609B8_PropagateChainState(ObjRecord *arg0);
void f_init_8015E7AC_ResolveChainedWeapon(ObjRecord *arg0);
void f_init_801604E0_SyncChainedWeapon(ObjRecord *arg0);

extern void func_8015E730(ObjRecord *arg0);   /* not yet decompiled */
void f_init_8015EF1C_LinkChainedWeapon(ObjRecord *arg0);
void f_init_8015F314_ResolveObjChain(ObjRecord *arg0);
extern void func_8015F6EC(ObjRecord *arg0);   /* not yet decompiled */
extern void f_init_80160060_ClearAndNotify(ObjRecord *arg0);
extern void func_80161254(void);               /* not yet decompiled */
extern void f_init_801619A0_SyncHolderIfSlotBusy(ObjRecord *arg0);
extern void f_init_80161EF8_SyncWeaponChain(ObjRecord *arg0);

/* The master per-command weapon dispatcher for the whole chain family:
   ties together f_init_8016112C_UpdateChainFlags, f_init_80161E28_
   UpdateWeaponTrack, f_init_8015EBB8_PropagateWeaponChain,
   f_init_801609B8_PropagateChainState, f_init_8015E7AC_
   ResolveChainedWeapon and f_init_801604E0_SyncChainedWeapon, plus several
   not-yet-decompiled siblings, selected primarily by `cmd` and secondarily
   by the object's own mode (unk26). Falls back to
   f_init_8015E7AC_ResolveChainedWeapon for any cmd/mode combination that
   doesn't match one of the explicit cases (including cmd == 0x56 or 0x5A,
   which instead just reset unk4 to -1 directly). Independently of the first
   dispatch, cmd 0x63/0x39 also runs f_init_801604E0_SyncChainedWeapon,
   cmd 0x2D/0x6A also runs f_init_80160060_ClearAndNotify, and mode 0xC also runs
   func_8015F6EC - these three are unconditional follow-ups, not part of
   the else-if chain. */
void f_init_801621D4_DispatchWeaponCommand(ObjRecord *arg0, s32 cmd) {
    u8 mode = arg0->mode;

    if (cmd == 0x49) {
        func_80161254();
    } else if (mode == 0xA) {
        f_init_8016112C_UpdateChainFlags(arg0, cmd);
    } else if (mode == 8 && cmd != 0xF) {
        f_init_8015F314_ResolveObjChain(arg0);
    } else if (mode == 0xE || (u32) (cmd - 0x20) < 2) {
        f_init_80161E28_UpdateWeaponTrack(arg0);
    } else if (mode == 5) {
        f_init_8015EBB8_PropagateWeaponChain(arg0);
    } else if (cmd == 9) {
        f_init_8015EF1C_LinkChainedWeapon(arg0);
    } else if (mode == 7) {
        f_init_80161EF8_SyncWeaponChain(arg0);
    } else if (cmd == 0x40) {
        f_init_801609B8_PropagateChainState(arg0);
    } else if (cmd == 0x55) {
        f_init_801619A0_SyncHolderIfSlotBusy(arg0);
    } else if (mode == 0xD || mode == 0x12) {
        if (cmd == 0x56 || cmd == 0x5A) {
            arg0->unk4 = (u32) -1;
        } else {
            f_init_8015E7AC_ResolveChainedWeapon(arg0);
        }
    } else if (cmd == 0x29) {
        func_8015E730(arg0);
    } else {
        f_init_8015E7AC_ResolveChainedWeapon(arg0);
    }

    if (cmd == 0x63 || cmd == 0x39) {
        f_init_801604E0_SyncChainedWeapon(arg0);
        return;
    }
    if (cmd == 0x2D || cmd == 0x6A) {
        f_init_80160060_ClearAndNotify(arg0);
        return;
    }
    if (arg0->mode == 0xC) {
        func_8015F6EC(arg0);
    }
}
