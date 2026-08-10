#include "../../include/types.h"

extern void f_main_80068C38_UpdateActors(void);
extern void f_main_80068D14_ForEachActor(void);
extern void f_main_80068D5C_ForEachActorCore(void);
extern void f_main_80068DA4_ForEachAngTrigger(void);
extern void f_main_80068E00_ForEachActorClearFlag(void);
extern void f_main_80068E64_ForEachActorTrigger160(void);
extern void f_main_80068EB8_ForEachActorMaybeTrigger(void);
extern void f_main_80069000_ForEachActorTriggerIf160(void);
extern void f_main_80069180_ForEachActorTrigger160Pair(void);
extern void f_main_80068F28_RunActorTriggerPass(void);
extern void f_main_80069054_UpdateActorPhysics(void);
extern void f_main_800691FC_ForEachActorTrigger10(void);
extern void f_main_8006924C_UpdateActorExpiry(void);
extern void f_main_80069308_ForEachActorFinal(void);

extern void func_800A01A8(s32 arg0); /* guess: begin(1)/end(0) bracket around the actor-list passes below, not yet decompiled */

/* Master per-frame actor update: runs every registered-actor pass in
   sequence. `arg0` isn't used - the original asm temporarily repoints $sp
   at a fixed PSX scratchpad address around several of these calls (a
   codegen/perf detail, not represented here). */
void f_main_80069350_RunActorFrameUpdate(void *arg0) {
    f_main_80068C38_UpdateActors();
    f_main_80068D14_ForEachActor();
    f_main_80068D5C_ForEachActorCore();

    func_800A01A8(1);
    f_main_80068DA4_ForEachAngTrigger();
    f_main_80068E00_ForEachActorClearFlag();
    f_main_80068E64_ForEachActorTrigger160();
    f_main_80068EB8_ForEachActorMaybeTrigger();
    f_main_80069000_ForEachActorTriggerIf160();
    f_main_80069180_ForEachActorTrigger160Pair();
    f_main_80068F28_RunActorTriggerPass();
    f_main_80069054_UpdateActorPhysics();
    f_main_800691FC_ForEachActorTrigger10();
    func_800A01A8(0);

    f_main_8006924C_UpdateActorExpiry();
    f_main_80069308_ForEachActorFinal();
}
