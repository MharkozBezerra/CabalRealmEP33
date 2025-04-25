#pragma once
#include "Base.h"
#include "GameDef.h"
#include "Memory/Memory.h"
#include "WindowObj/WindowObj.h"
#include "cDescription/cDescription.h"

struct sBattleMode3
{
	char NamePrefix[16];
	int PrefixSize;
	int PrefixMaxSize;
	int Combo[4];
};

struct sSinergyProcInfo
{
	int Unk;
	int Unk2;
	int Id;
	int Level;
};

struct sSinergyBuff
{
	int force_code;
	int force_value;
	int force_type;
	int target_type;
};

struct sSinergyList
{
	int Id;
	std::string Combo;
	std::vector<sSinergyBuff> vBuffList;
};

struct sPlayerSinergy
{
	std::string Name;
	std::string Combo;
	int Id;
	int Level;
	BOOL Proc;
	std::vector<std::pair<std::string, std::string>> vPlayerSinergyBuff;
};

class BM3Sinergy : public sWindowObject
{
public:
	BM3Sinergy(int x, int y, int w, int h);
	void LoadJsonInfos();
	void Draw();
	void DrawBuff();
	void Save() {};
	void SetPlayerSinergy();
	BOOL CloseWithKey() { return FALSE; };
	BOOL ObjectClick(int x, int y) { return FALSE; };
	BOOL OnHover(int x, int y) { return FALSE; };
	BOOL ScreenVisible();

	static void __fastcall SinergyProc(sSinergyProcInfo* pSinergyInfo);
	static BYTE __fastcall SinergyRemove(void* This, void* n, int SinergyId);

	sCabalWindow* pCabalWindow = nullptr;
	sCabalWindow* pSecondWindow = nullptr;

	std::map<int, int> mForceList
	{
		{eForceList::E_FORCE_NONE, 0},
		{eForceList::E_FORCE_HP, 76},
		{eForceList::E_FORCE_MP, 77},
		{eForceList::E_FORCE_ATTACK, 31},
		{eForceList::E_FORCE_MAGIC_ATTACK, 32},
		{eForceList::E_FORCE_DEFENSE, 33},
		{eForceList::E_FORCE_ATTACK_RATE, 34},
		{eForceList::E_FORCE_DEFENSE_RATE, 35},
		{eForceList::E_FORCE_CRITICAL_DMG, 53},
		{eForceList::E_FORCE_CRITICAL_RATE, 100},
		{eForceList::E_FORCE_MIN_DMG, 276},
		{eForceList::E_FORCE_MAX_HP_STEAL, 280},
		{eForceList::E_FORCE_MAX_MP_STEAL, 281},
		{eForceList::E_FORCE_MAX_CRITICAL_RATE, 275},
		{eForceList::E_FORCE_SWORD_SKILL_AMP, 288},
		{eForceList::E_FORCE_MAGIC_SKILL_AMP, 289},
		{eForceList::E_FORCE_HP_STEAL, 278},
		{eForceList::E_FORCE_MP_STEAL, 279},
		{eForceList::E_FORCE_FLEE_RATE, 284},
		{eForceList::E_FORCE_HP_RESTORE, 211},
		{eForceList::E_FORCE_MP_RESTORE, 212},
		{eForceList::E_FORCE_ADD_DMG, 277},
		{eForceList::E_FORCE_UNK_54, 0},
		{eForceList::E_FORCE_EVADE_DEBUFF_SKILL, 1051},
		{eForceList::E_FORCE_UNK_56, 0},
		{eForceList::E_FORCE_LEVEL_DOWN, 533},
		{eForceList::E_FORCE_STUN, 534},
		{eForceList::E_FORCE_HEAL, 535},
		{eForceList::E_FORCE_DOWN, 536},
		{eForceList::E_FORCE_KNOCK_BACK, 537},
		{eForceList::E_FORCE_UNMOVABLE, 538},
		{eForceList::E_FORCE_ABSORB_DMG, 539},
		{eForceList::E_FORCE_REFLECT_DMG, 540},
		{eForceList::E_FORCE_SWORD_1, 561},
		{eForceList::E_FORCE_SWORD_2, 561},
		{eForceList::E_FORCE_SWORD_3, 561},
		{eForceList::E_FORCE_SWORD_4, 561},
		{eForceList::E_FORCE_MAGIC_1, 562},
		{eForceList::E_FORCE_MAGIC_2, 562},
		{eForceList::E_FORCE_MAGIC_3, 562},
		{eForceList::E_FORCE_ALLDMG_REDUCE, 1381},
		{eForceList::E_FORCE_HIT, 1382},
		{eForceList::E_FORCE_MAGIC_3, 562},
		{eForceList::E_FORCE_RATE_OF_ASTRAL_BUFF, 541},
		{eForceList::E_FORCE_NUMBER_OF_ASTRAL_BUFF, 542},
		{eForceList::E_FORCE_ATTACK_DISTANCE, 543},
		{eForceList::E_FORCE_STRENGTHEN_CURSE, 544},
		{eForceList::E_FORCE_DEBUFF, 545},
		{eForceList::E_FORCE_INCREASE_SP_REGEN, 546},
		{eForceList::E_FORCE_DECREASE_SP_USE, 547},
		{eForceList::E_FORCE_INTERCEPT, 548},
		{eForceList::E_FORCE_PUT_TO_SLEEP, 549},
		{eForceList::E_FORCE_INCREASE_AGGRO, 550},
		{eForceList::E_FORCE_SP_ABSORB_UP, 551},
		{eForceList::E_FORCE_HP_DOWN, 552},
		{eForceList::E_FORCE_MP_DOWN, 553},
		{eForceList::E_FORCE_RESIST_CRITICAL_RATE, 554},
		{eForceList::E_FORCE_RESIST_CRITICAL_DMG, 555},
		{eForceList::E_FORCE_RESIST_UNABLE_TO_MOVE, 556},
		{eForceList::E_FORCE_RESIST_SLIP, 557},
		{eForceList::E_FORCE_RESIST_DOWN, 558},
		{eForceList::E_FORCE_RESIST_KNOCK_BACK, 559},
		{eForceList::E_FORCE_RESIST_STUN, 560},
		{eForceList::E_FORCE_UNK_104, 0},
		{eForceList::E_FORCE_RESIST_SKILL_AMP, 952},
		{eForceList::E_FORCE_ALL_ATTACK, 967},
		{eForceList::E_FORCE_ALL_SKILL_AMP, 969},
		{eForceList::E_FORCE_HEAL2, 535},
	};

	std::map<int, std::vector<sSinergyList>> mSinergyList;
	std::vector<std::shared_ptr<sPlayerSinergy>> vPlayerSinergy;
	std::shared_ptr<cDescription> pDescription = nullptr;
};

extern std::shared_ptr<BM3Sinergy> pBM3Sinergy;