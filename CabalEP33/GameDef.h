#pragma once
#include "Base.h"
#include <d3dx9.h>

#define MaxResolutionX  *(int*)0xCEB83C 
#define MaxResolutionY  *(int*)0xCEB840
#define ObjectScaleX    *(float*)0xCDA3BC
#define ObjectScaleY    *(float*)0xCDA3C0
#define ObjectScaleZ    *(float*)0xBB38A4

enum eChatColorType : int
{
	E_CHAT_COLOR_NORMAL,
	E_CHAT_COLOR_PARTY,
	E_CHAT_COLOR_GUILD,
	E_CHAT_COLOR_WHISPER,
	E_CHAT_COLOR_SHOUT,
	E_CHAT_COLOR_CHANNEL,
	E_CHAT_COLOR_MEGAFONE = 34
};

enum TextStyle
{
	system_only = 0,
	green_screencenter = 7,
	red_screencenter = 19,
	white_screencenter = 21,
	yellow_screencenter = 22,
	medium_screencenter_icewhite = 25,
	medium_screencenter_blue = 26,
	medium_screencenter_red = 27,
	big_screencenter_pink = 28,
	big_screencenter_lightblue = 37,
	big_screencenter_lightyellow = 38
};

enum eItemOrigin : int
{
	e_ORIGIN_INVALID,
	e_ORIGIN_EQUIPMENT,
	e_ORIGIN_INVENTORY,
	e_ORIGIN_WHAREHOUSE
};

enum eInventoryItems
{
	E_Helm,
	E_Armor,
	E_Gloves,
	E_Boots,
	E_WRHand,
	E_WLHand,
	E_Epaulet,
	E_Amulet,
	E_RingTopLeft,
	E_RingTopRight,
	E_Bike,
	E_Pet,
	E_Hide,
	E_EarringLeft,
	E_EarringRight,
	E_BracLeft,
	E_BracRight,
	E_RingBottomLeft,
	E_RingBottomRight,
	E_Belt,
	E_Pet_Extend,
	E_Talisman,
	E_EffectorTopLeft,
	E_EffectorTopRight,
	E_Memorial,
	E_Fantasy_WEAPON,
	E_Fantasy_WEAPON_2,
	E_Fantasy_HEAD,
	E_Fantasy_STATUS_ARMOR,
	E_BubbleChat,
	E_Talisman_OfChaos,
	E_Fantasy_ARMOR,
	E_Carnelian,
	E_ArcanaLeft,
	E_ArcanaRight,
	E_Fantasy_STATUS_BIKE,
	E_Fantasy_BIKE,
	E_Fantasy_STATUS_WINGS,
	E_Fantasy_WINGS,
	E_Pet_Show,
	E_Nevareth_Honor,
	E_Cube_1,
	E_Cube_2,
	E_Cube_3,
	E_Brooch,
	E_EffectorBottomLeft,
	E_EffectorBottomRight,
	E_START_INV_SLOT = 47,
	E_MAX_INV_ITEM = 558,
	E_MOUSE = 569,
	MAX_INVENTORYTAB = 64,
};

#define EVENT_ALERT_DRAW_CHAT_MSG
#define MASK_ITEMKINDINDEX		0x6000FFF
#define PVP_SERVER_ID           99

#define CabalDrawText  ((void(__thiscall*)(void* Pointer, const char* Msg, TextStyle Style, int TType, int ))0x00730205) //this = 0x00CFA638
#define DrawTextByFile ((char*(__thiscall*)(void* Pointer, int Line, TextStyle Style, int TType))0x0073403F)
#define DrawProcMsg    ((void(__thiscall*)(void* Pointer, const char* Msg, TextStyle Style, int TType))0x0072FA3D)
#define C2SLoudMsg     ((void(__thiscall*)(void* Pointer, const char* Msg, int TType))0x0073AEC5)
#define CheckTG ((bool(__cdecl*)(DWORD MapCheck))0x009B6E26)

#ifdef DEV_MODE
const static int MAX_GAME_INSTANCE = 6;
const std::string GameWndName = "Cabal Realm EP33 - Ingens Proelium [DEV MODE]";
#else
const static int MAX_GAME_INSTANCE = 1;
const std::string GameWndName = "Cabal Realm EP33 - Ingens Proelium";
#endif

#ifndef XIGNCODE_LOAD_X3_XEM
constexpr auto XIGNCODE_LOAD_METHOD = 8;
#else
constexpr auto XIGNCODE_LOAD_METHOD = 1;
#endif

template <typename T> T new_game(SIZE_T dwSize)
{
	typedef T(*t_new)(SIZE_T dwSize);
	t_new new_game_ = (t_new)0x00AE24E4;

	T ptr = new_game_(dwSize);
	memset(ptr, 0, dwSize);
	return ptr;
}

enum eForceList : int
{
	E_FORCE_NONE,
	E_FORCE_HP,
	E_FORCE_MP,
	E_FORCE_ATTACK,
	E_FORCE_MAGIC_ATTACK,
	E_FORCE_DEFENSE,
	E_FORCE_ATTACK_RATE,
	E_FORCE_DEFENSE_RATE,
	E_FORCE_CRITICAL_DMG,
	E_FORCE_CRITICAL_RATE,
	E_FORCE_MIN_DMG,
	E_FORCE_MAX_HP_STEAL = 12,
	E_FORCE_MAX_MP_STEAL,
	E_FORCE_MAX_CRITICAL_RATE = 25,
	E_FORCE_SWORD_SKILL_AMP = 30,
	E_FORCE_MAGIC_SKILL_AMP = 32,
	E_FORCE_HP_STEAL = 34,
	E_FORCE_MP_STEAL,
	E_FORCE_FLEE_RATE,
	E_FORCE_HP_RESTORE,
	E_FORCE_MP_RESTORE,
	E_FORCE_ADD_DMG,
	E_FORCE_UNK_54 = 54,
	E_FORCE_EVADE_DEBUFF_SKILL,
	E_FORCE_UNK_56,
	E_FORCE_LEVEL_DOWN = 61,
	E_FORCE_STUN,
	E_FORCE_HEAL,
	E_FORCE_DOWN,
	E_FORCE_KNOCK_BACK,
	E_FORCE_UNMOVABLE,
	E_FORCE_ABSORB_DMG,
	E_FORCE_REFLECT_DMG,
	E_FORCE_SWORD_1,
	E_FORCE_SWORD_2,
	E_FORCE_SWORD_3,
	E_FORCE_SWORD_4,
	E_FORCE_MAGIC_1,
	E_FORCE_MAGIC_2,
	E_FORCE_MAGIC_3,
	E_FORCE_ALLDMG_REDUCE = 78,
	E_FORCE_HIT = 79,
	E_FORCE_RATE_OF_ASTRAL_BUFF = 80,
	E_FORCE_NUMBER_OF_ASTRAL_BUFF,
	E_FORCE_ATTACK_DISTANCE,
	E_FORCE_STRENGTHEN_CURSE = 84,
	E_FORCE_DEBUFF,
	E_FORCE_INCREASE_SP_REGEN = 88,
	E_FORCE_DECREASE_SP_USE,
	E_FORCE_INTERCEPT = 91,
	E_FORCE_PUT_TO_SLEEP,
	E_FORCE_INCREASE_AGGRO,
	E_FORCE_SP_ABSORB_UP,
	E_FORCE_HP_DOWN,
	E_FORCE_MP_DOWN,
	E_FORCE_RESIST_CRITICAL_RATE,
	E_FORCE_RESIST_CRITICAL_DMG,
	E_FORCE_RESIST_UNABLE_TO_MOVE,
	E_FORCE_RESIST_SLIP,
	E_FORCE_RESIST_DOWN,
	E_FORCE_RESIST_KNOCK_BACK,
	E_FORCE_RESIST_STUN,
	E_FORCE_UNK_104,
	E_FORCE_RESIST_SKILL_AMP = 112,
	E_FORCE_ALL_ATTACK,
	E_FORCE_ALL_SKILL_AMP,
	E_FORCE_HEAL2 = 159
};

enum BattleModeType
{
	None = 0x0,
	Astral = 0x02,
	Aura = 0x08,
	BM1 = 0x10,
	BM2 = 0x20,
	BM3 = 0x40
};

enum eClassCode
{
	GU = 1,
	DU = 2,
	MA = 3,
	AA = 4,
	GA = 5,
	EA = 6,
	GL = 7,
	FG = 8,
	DM = 9
};

struct sCabalWindow
{
	BYTE GetWindowStat() { return *(BYTE*)&((char*)this)[0x8D]; };
	int GetObjectPosX() { return *(int*)&((char*)this)[0x30]; };
	int GetObjectPosY() { return *(int*)&((char*)this)[0x34]; };
	int GetObjectbX() { return *(int*)&((char*)this)[0x38]; };
	int GetObjectbY() { return *(int*)&((char*)this)[0x3c]; };
	int GetObjecttX() { return *(int*)&((char*)this)[0x40]; };
	int GetObjecttY() { return *(int*)&((char*)this)[0x44]; };
	int GetObjectW() { return *(int*)&((char*)this)[0x48]; };
	int GetObjectH() { return *(int*)&((char*)this)[0x4C]; };
	int GetInventoryTabIdx() { return *(int*)&((char*)this)[0x13C]; };
};

struct sItemClick
{
	eItemOrigin Type;
	int ClickedSlot;
	int Unk;
	eItemOrigin LastType;
	int LastClickedSlot;
	int LastUnk;
};

struct sSinergyInfo
{
	int SinergyCount;
	int SinergyID[3];
	int SinergyHitCount;
	int SinergyHitId[4];
};

struct sItemDescription
{
	char ItemContent[32];
};

struct sPetInfo
{
	int PetisSeal;
	int PetShowSlot;
	int PetUniqId;
};

struct sItemReqStatus
{
	int STR;
	int DES;
	int INT;
};

struct sItemSlotUse
{
	int X;
	int Y;
};

struct sObjectNameInfo
{
	char ItemAmbiguousName[16];
	int  ItemNameSize;
	int  ItemNameAllocSpace;
};

struct sItemInfo
{
	char			bUnk_1[0xC];
	int				ItemId;
	sItemSlotUse	ItemSlotUse;
	char			bUnk_2[0xD8];
	sObjectNameInfo ItemNameInfo;
	char			bUnk3[0x24];
	int				ItemType;

};

struct sItem
{
	char			bUnk_1[0xC];
	sPetInfo		PetInfo;
	sItemInfo* psItemInfo;
	char			bUnk_2[0x4];
	sItemInfo* pItemMogInfo;
	char			bUnk_3[0x1C];
	int				ItemUpgrade;
	int				ExtremeUpgrade;
	int				DivineUpgrade;
	int				ItemAccountBound;
	int				ItemCharBound;
	int				ItemCharBoundWhenEquip;
	int				ItemOnlyUseIfPcCafe;
	int				ItemOnSpecialInventory;
	int				ItemColor;
	int				ItemUnk;
	INT64			ItemOpt;
	int				ItemImageShow;
	sItemReqStatus	ItemReqStatus;
	char			bUnk_4[0x60];
	BOOL			bUnlocked;
	int				FilledSlot;
	int				SlotCount;
};

struct sItemSlot
{
	int Type;
	int Slot;
};

struct sInventorySlot
{
	sItem** pAllItems;
};

struct sPutItemOnUI
{
	int Flag;
	int Slot;
	int UiIdx;

	sPutItemOnUI() : Flag(2), Slot(-1), UiIdx(-1) {}
	sPutItemOnUI(int iSlot, int iUI) : Slot(iSlot), UiIdx(iUI), Flag(2) {}
};

class USERDATACONTEXT
{
public:
	static USERDATACONTEXT* GetpUserDataCtx()
	{
		if (USERDATACONTEXT** pUserDataCtx = reinterpret_cast<USERDATACONTEXT**>(0x00CECAC4))
			return *pUserDataCtx;

		return nullptr;
	}
	int*** GetUiInfomation() { return (int***)&((char*)this)[0x5F44]; };
	sSinergyInfo* GetSinergyInfo() { return (sSinergyInfo*)&((char*)this)[0xC80]; };
	sItemClick* GetItemClick() { return (sItemClick*)&((char*)this)[0x5F20]; };
	sInventorySlot* GetpSlot() { return (sInventorySlot*)&((char*)this)[0x113C]; };
	DWORD GetCharacterIdx() { return *(int*)&((char*)this)[0x394]; };
	int GetCharacterNation() { return *(int*)&((char*)this)[0x4A4]; };
	int GetCharacterClass() { return *(int*)&((char*)this)[0x280C]; };
	int GetCharacterRank() { return *(int*)&((char*)this)[0x2810]; };
	int GetCharacterStyle() { return *(int*)&((char*)this)[0x283C]; };
	int GetCharacterLevel() { return *(int*)&((char*)this)[0x2D90]; };
	int GetGuildLevel() { return *(int*)&((char*)this)[0x5644]; };
	int GetMapId() { return *(int*)&((char*)this)[0x5710]; };
	int GetOnLogged() { return *(int*)&((char*)this)[0x2F58]; };
	void SetOnLogged(int Logged) { *(int*)&((char*)this)[0x2F58] = Logged; }
	int GetDungeonIdx() { return *reinterpret_cast<int*>(0x0110E1E8); }

	BYTE* GetBattleType() { return (BYTE*)&((char*)this)[0x2844]; };
	int GetCurBattleBarStats() { return *(int*)&((char*)this)[0x301C]; };
	int GetBattleBarStats() { return *(int*)&((char*)this)[0x394]; };

	char* GetCharacterName() { return (char*)&((char*)this)[0x2528]; }
	int GetCharacterNameLen() { return *(int*)&((char*)this)[0x2538]; }
	int GetCharacterNameAlloc() { return *(int*)&((char*)this)[0x253C]; }
	char* GetGuildName() { return (char*)&((char*)this)[0x24D0]; }
	int	 GetGuildNameLen() { return *(int*)&((char*)this)[0x24E0]; }
	int GetGuildNameAlloc() { return *(int*)&((char*)this)[0x24E4]; }
	int	 GetAction() { return *(int*)&((char*)this)[0x2840]; }
	static int GetSinergyProcCount() { return *reinterpret_cast<int*>(0x00CFA8EC); };
	std::pair<int, int> GetBattleModeWindowId() { return GetCurBattleBarStats() != GetBattleBarStats() ? std::make_pair(3700000, 3700040) : std::make_pair(3800000, 3800041); }// ALLBMS (Aura = ID -1)


	double GetReleaseSkillTime() { return *(double*)&((char*)this)[0x1100]; };
	double GetSkillTime() { return *(double*)&((char*)this)[0x98]; };


	void SetReleaseSkillTime(double Time)
	{
		*(double*)&((char*)this)[0x1100] = Time;
	}

	sItem* getCursorItem()
	{
		return GetpSlotItem(E_MOUSE);
	}

	sItem* GetpSlotItem(int slotItem)
	{
		if (sInventorySlot* pInventorySlot = GetpSlot())
			return pInventorySlot->pAllItems[slotItem];

		return nullptr;
	}

	void SetpSlotItem(sItem* pItem, int slotItem)
	{
		if (sInventorySlot* pInventorySlot = GetpSlot())
		{
			if (pInventorySlot->pAllItems[slotItem])
				pInventorySlot->pAllItems[slotItem] = pItem;
		}
	}

	static D3DCOLOR getGameColorById(int id)
	{
		static D3DCOLOR* allGameColors = (D3DCOLOR*)0x115D1AC;

		return allGameColors[id];
	}
private:

};