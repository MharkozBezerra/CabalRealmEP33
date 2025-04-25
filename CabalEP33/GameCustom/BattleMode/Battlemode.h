#pragma once
#include "Base.h"
#include "GameDef.h"
#include "GameCustom/Natives/CSkillEntity.h"

namespace BattleMode
{
	bool IsIstantlyBattlemode(int timeIdx);
	void __fastcall UseBattleMode1(USERDATACONTEXT* pUserDataCtx, void* edx, int timeIdx);
	void __fastcall UseBattleMode2(USERDATACONTEXT* pUserDataCtx, void* edx, int timeIdx);
	void __fastcall UseBattleMode3(USERDATACONTEXT* pUserDataCtx, void* edx, int timeIdx);

	void SetHooks();
}