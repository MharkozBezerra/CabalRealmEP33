#pragma once
#include "Base.h"
#include "GameDef.h"
#include "Memory/Memory.h"
#include <codecvt>

#define GAME_TIMER			(*(double**)0x00CE7EAC)[0x18E8 / sizeof(double)]

#pragma pack( push, 1 )
struct sItemTransformName
{
	char Name[16];
	int NameSize;
	int AllocNameSize;
};

struct sChannelInfo
{
	int First;
	char Name[16];
	int StrSize;
	int StrAllocSize;
};

struct sLobbyStatus
{
	char Unk[8];
	int Step_1;
	int Step_2;
	char Unk2[320];
	float posX;
	float posY;
	float fUnk1;
	float posZ;
};
#pragma pack( pop )

namespace Basic
{
	static const unsigned int MAX_DEL_LEVEL = 100;

	int __fastcall OnLogged(USERDATACONTEXT* pUserDataCtx, void* n, int Status);
	int GetVisualConfig(int Type);
	void DlgDelChar(char* Dlg, char* p2);
	void SetHooks();

	namespace Game
	{
		static std::string GameDir;
		static int GetClassId(USERDATACONTEXT* pUserDataCtx) { return pUserDataCtx->GetCharacterClass(); }
		static int GetCharLvL(USERDATACONTEXT* pUserDataCtx) { return pUserDataCtx->GetCharacterLevel(); }
		static int GetCharGender(USERDATACONTEXT* pUserDataCtx) { return pUserDataCtx->GetCharacterStyle() & 0x4000000; };
		static int GetMapId(USERDATACONTEXT* pUserDataCtx) { return pUserDataCtx->GetMapId(); }

		static int GetChannelId() { return *reinterpret_cast<int*>(0x01104950); }
		static int GetServerId() { return *reinterpret_cast<int*>(0x110494C); }
		static bool isPvPBattle() { return *reinterpret_cast<bool*>(0xCDB6FC); }
		static int GetPartyCount() { return *reinterpret_cast<int*>(0x01112B40); }

		std::string GetCharName();
		std::string GetGuildName();
		std::string GetMapName(USERDATACONTEXT* pUserDataCtx, int MapId = 0, BOOL bUtf8 = FALSE);
		std::string GetChannelName(int ChannelId = FALSE);
		std::string GetGameDir();
		bool EnsureDir(std::string dir);

		INT64 GetFileSize(std::string filePath);
		INT64 GetFileSize(std::string path, std::string filename);

		int LobbyType();


		namespace Item
		{
			sItem* GetpItem(INT64 ItemId);
			int TransformpItem(sItem* pItem, INT64 ItemId, INT64 ItemOpt);
			int GetCodeColor(sItem* pItem);
			char* GetBasicName(sItem* pItem);
			char* GetCompleteName(sItem* pItem);
			char* GetFullName(sItem* pItem, BOOL bColor = FALSE);
			void MoveItemToUi();
		}
	}

	namespace Useful
	{
		std::string to_utf8(const std::string& str, const std::locale& loc = std::locale{});
		std::string from_utf8(const std::string& str, const std::locale& loc = std::locale{});

		std::string ConcatValue(INT64 Value);
		std::pair<std::string, int> getTextAndColor(std::string itemName);
		std::string FindAndReplace(std::string input, std::string find, std::string replace);
	}
}