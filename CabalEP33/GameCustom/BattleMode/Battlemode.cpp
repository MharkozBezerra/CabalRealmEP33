#include "Battlemode.h"
#include "Memory/Memory.h"

bool BattleMode::IsIstantlyBattlemode(int timeIdx)
{
    VM_STARTM(__FUNCTION__);

    typedef CSkillStuff* (__thiscall* t_GetSkillPtr)(void*, int);
    t_GetSkillPtr GetSkillPtr = (t_GetSkillPtr)0x009E9EE4;

    CSkillStuff* pSkillData = GetSkillPtr(reinterpret_cast<void*>(0x1183D88), timeIdx);
    return (pSkillData && pSkillData->iGroup == 32) ? true : false;
    VM_END;
}

void __fastcall BattleMode::UseBattleMode1(USERDATACONTEXT* pUserDataCtx, void* edx, int timeIdx)
{
    auto OrgExecBM1 = reinterpret_cast<void(__thiscall*)(USERDATACONTEXT*, int)>(0x006957DA);

    OrgExecBM1(pUserDataCtx, timeIdx);

    if (IsIstantlyBattlemode(timeIdx))
        pUserDataCtx->SetReleaseSkillTime(pUserDataCtx->GetSkillTime());
}

void __fastcall BattleMode::UseBattleMode2(USERDATACONTEXT* pUserDataCtx, void* edx, int timeIdx)
{
    auto OrgExecBM2 = reinterpret_cast<void(__thiscall*)(USERDATACONTEXT*, int)>(0x006AAE81);

    OrgExecBM2(pUserDataCtx, timeIdx);

    if (IsIstantlyBattlemode(timeIdx))
        pUserDataCtx->SetReleaseSkillTime(pUserDataCtx->GetSkillTime());
}

void __fastcall BattleMode::UseBattleMode3(USERDATACONTEXT* pUserDataCtx, void* edx, int timeIdx)
{
    auto orgExecBM3 = reinterpret_cast<void(__thiscall*)(USERDATACONTEXT*, int)>(0x006AB1CB);
        
    orgExecBM3(pUserDataCtx, timeIdx);

    if (IsIstantlyBattlemode(timeIdx))
        pUserDataCtx->SetReleaseSkillTime(pUserDataCtx->GetSkillTime());
}

void BattleMode::SetHooks()
{
    //BM1
	Memory::HookFunc<Memory::HookType::CALL>(UseBattleMode1, 0x0071140D);
	Memory::WriteValue(0x00BC09D4, UseBattleMode1);
    
    //BM2
    Memory::HookFunc<Memory::HookType::CALL>(UseBattleMode2, 0x0071522B);
	Memory::WriteValue(0x00BC09D8, UseBattleMode2);

    //BM3
    Memory::HookFunc<Memory::HookType::CALL>(UseBattleMode3, 0x007154C8);
	Memory::WriteValue(0x00BC09DC, UseBattleMode3);
}
