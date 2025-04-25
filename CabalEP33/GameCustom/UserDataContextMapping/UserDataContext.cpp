#include "UserDataContext.h"


extern SKILLMANAGER* g_pSkillManager = reinterpret_cast<SKILLMANAGER*>(0x00CD9688);

int USERDATACONTEXT_EXT::GetCurrentSkill()
{
	return currentUsedSkill;
}

void USERDATACONTEXT_EXT::SetCurrentSkill(int skillIdx)
{
	currentUsedSkill = skillIdx;
}

//aparentemente a skilldata tem 388 bytes
SKILL_DATA* USERDATACONTEXT_EXT::GetSkillPtr(int skillIdx)
{
	return reinterpret_cast<SKILL_DATA*(__thiscall*)(unsigned int _this, int skillIndex)>(0x009E9EE4)(0x1183D88, skillIdx);
}

int USERDATACONTEXT_EXT::GetDungeonIdx()
{
	//00D1BF54 -> aqui é -1 ou id da dg
	return *reinterpret_cast<int*>(0x0110E1E8);
}

void USERDATACONTEXT_EXT::ExecuteSkill(int slotIdx)
{
	reinterpret_cast<void(__thiscall*)(USERDATACONTEXT_EXT*, int)>(0x00717EF2)(this, slotIdx);
}

INT64 USERDATACONTEXT_EXT::GetGameTime()
{
	return (*(double**)0x00CE7EAC)[0x18E8 / sizeof(double)];
}

void SKILLMANAGER::ClearSkillCooldown(int skillIdx)
{
	if (auto pSkillData = MyUserCtx->GetSkillPtr(skillIdx))
	{
		double curTimeGame = MyUserCtx->GetGameTime();

		auto tmpNode = g_pSkillManager->tree_skillCooldown;

		if (auto resultTree = g_pSkillManager->FindSkillByCooldownIdx(pSkillData->cooltimeIdx))
		{
			resultTree->data.lastUse        = curTimeGame + 5;
			resultTree->data.skillDuration  = 80.0f;
		}
	}
}
