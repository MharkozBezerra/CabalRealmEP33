#pragma once
#include "Base.h"
#include "Basic/Basic.h"
#include "discord-game-sdk\\discord.h"
#include <thread>

enum eGAME_STATE : BYTE
{
	GAME_STATE_START,
	GAME_STATE_ONLOBBY,
	GAME_STATE_ONCREATE_CHARACTER,
	GAME_STATE_PLAYING,
	GAME_STATE_FINISH
};

namespace Discord
{
	extern discord::Core* pCore;
	extern eGAME_STATE GameState;
	extern time_t InGameTimestamp;
	extern std::vector<std::string> vDiscordClassList;
	extern std::vector<std::string> vNationList;
	extern std::map<eGAME_STATE, std::string> mGameState;

	DWORD WINAPI CreateDiscord();
	static const char* GetGameState() { return mGameState.find(GameState)->second.c_str(); }
	void SetGameState(eGAME_STATE State);
	void UpdateActivity();
	void CallBack_UpdateActivity(discord::Result res);

	namespace Hook
	{
		void UpdateMap(void* pPacket);
		void __fastcall PartyInc(void* PtMembersCount, void* n, int CharIdx);
		void __fastcall PartyDec(void* PtMembersCount, void* n, int CharIdx);
		void __fastcall PartyZero(void* PtMembersCount);
		void __fastcall SetLobbyState(void* This, void* n, int Flag);
		void __fastcall SetGuild(void* pGuild, void* n, char* pGuildName, int Flag);
		void __fastcall SetLFGAction(void* This, void* n, int a1, int a2, int a3);
	}

	void SetHooks();
}