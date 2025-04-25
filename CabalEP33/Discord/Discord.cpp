#include "Base.h"
#include "Discord.h"
#include <csignal>
#include <format>
#include <sstream>

using namespace Basic;
discord::Core* Discord::pCore = nullptr;
eGAME_STATE Discord::GameState = eGAME_STATE::GAME_STATE_START;
time_t Discord::InGameTimestamp = 0;

std::vector<std::string> Discord::vDiscordClassList =
{
	{"Desconhecido"},
	{"Guerreiro"},
	{"Duelista"},
	{"Mago"},
	{"Arqueiro Arcano"},
	{"Guardião Arcano"},
	{"Espadachim Arcano"},
	{"Gladiador"},
	{"Atirador Arcano"},
	{"Mago Negro"}
};

std::vector<std::string> Discord::vNationList
{
	{"Neutro"},
	{"Capella"},
	{"Procyon"},
	{"GM"}
};

std::map<eGAME_STATE, std::string> Discord::mGameState =
{
	{eGAME_STATE::GAME_STATE_START, "Jogo Iniciado"},
	{eGAME_STATE::GAME_STATE_ONLOBBY, "Na tela de seleção de personagem"},
	{eGAME_STATE::GAME_STATE_ONCREATE_CHARACTER, "Na tela de criação de personagem"},
	{eGAME_STATE::GAME_STATE_PLAYING, ""},
	{eGAME_STATE::GAME_STATE_FINISH, "Jogo Finalizado"},

};

DWORD __stdcall Discord::CreateDiscord()
{						  //DiscordApplicationId
	discord::Core::Create(1029148748225462322, DiscordCreateFlags_NoRequireDiscord, &pCore);
	if (pCore)
	{
		InGameTimestamp = std::time(nullptr);
		SetGameState(eGAME_STATE::GAME_STATE_START);
		signal(SIGINT, [](int) { GameState = eGAME_STATE::GAME_STATE_FINISH; });

		while (GameState != eGAME_STATE::GAME_STATE_FINISH)
		{
			pCore->RunCallbacks();
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		}
	}

	return FALSE;
}

void Discord::SetGameState(eGAME_STATE State)
{
	GameState = State;
	UpdateActivity();
}

void Discord::UpdateActivity()
{
	if (pCore && GameState != eGAME_STATE::GAME_STATE_FINISH)
	{
		discord::Activity activity{};

		if (GameState == eGAME_STATE::GAME_STATE_PLAYING)
		{
			if (USERDATACONTEXT* pUserDataCtx = USERDATACONTEXT::GetpUserDataCtx())
			{
				if (Game::GetServerId() != PVP_SERVER_ID)
					activity.SetDetails(std::format("{} - {}", Game::GetMapName(pUserDataCtx, 0, TRUE), Game::GetChannelName()).c_str());
				else
					activity.SetDetails(std::format("{}", Game::isPvPBattle() ? "PvP Battle" : "Mission Battle").c_str());

				activity.GetAssets().SetLargeImage(std::format("bg_{}",  pUserDataCtx->GetCharacterClass()).c_str());

				int Action = (pUserDataCtx->GetAction() & 128) != 0 ? TRUE : FALSE;
				std::stringstream ss;
				ss << (pUserDataCtx->GetGuildNameLen() ? std::format("<{}> {}", Game::GetGuildName(), Game::GetCharName().c_str()) : std::format("{}", Game::GetCharName().c_str()));
				activity.GetAssets().SetLargeText(ss.str().c_str());

				activity.SetState(std::format("Nv.{} [{}] - {}", Game::GetCharLvL(pUserDataCtx), vNationList[pUserDataCtx->GetCharacterNation()], (Game::GetPartyCount() ? "Grupo" : Action ? "Solo [LFG]" : "Solo")).c_str());
				
				activity.GetParty().GetSize().SetCurrentSize(Game::GetPartyCount());
				activity.GetParty().GetSize().SetMaxSize(Game::GetPartyCount() ? 7 : 0);

				activity.GetAssets().SetSmallImage(std::to_string(Game::GetClassId(pUserDataCtx)).c_str());
				activity.GetAssets().SetSmallText(std::format("{} - Grau {}", vDiscordClassList[Game::GetClassId(pUserDataCtx)], pUserDataCtx->GetCharacterRank()).c_str());
			}
		}
		else
		{
			activity.SetDetails(Useful::to_utf8(GetGameState()).c_str());
			activity.GetAssets().SetLargeImage("logo");
			activity.GetAssets().SetLargeText(Useful::to_utf8("Cabal Neo").c_str());
		}

		activity.GetTimestamps().SetStart(InGameTimestamp);
		activity.SetType(discord::ActivityType::Playing);
		pCore->ActivityManager().UpdateActivity(activity, &Discord::CallBack_UpdateActivity);
	}
}

void Discord::CallBack_UpdateActivity(discord::Result res)
{
	if (res != discord::Result::Ok)
		SetGameState(eGAME_STATE::GAME_STATE_PLAYING);
}

void Discord::Hook::UpdateMap(void* pPacket)
{
	typedef void(__cdecl* TCSC_WARPCOMMAND)(void*);
	TCSC_WARPCOMMAND CSC_WARPCOMMAND = (TCSC_WARPCOMMAND)0x994EC4;

	CSC_WARPCOMMAND(pPacket);
	SetGameState(eGAME_STATE::GAME_STATE_PLAYING);
}

void __fastcall Discord::Hook::PartyInc(void* PtMembersCount, void* n, int CharIdx)
{
	typedef void(__thiscall* TPartyInc)(void*, int);
	TPartyInc PartyInc = (TPartyInc)0x007A61CC;

	PartyInc(PtMembersCount, CharIdx);
	SetGameState(eGAME_STATE::GAME_STATE_PLAYING);
}

void __fastcall Discord::Hook::PartyDec(void* PtMembersCount, void* n, int CharIdx)
{
	typedef int(__thiscall* TPartyDec)(void*, int);
	TPartyDec PartyDec = (TPartyDec)0x007A673D;

	PartyDec(PtMembersCount, CharIdx);
	SetGameState(eGAME_STATE::GAME_STATE_PLAYING);
}

void __fastcall Discord::Hook::PartyZero(void* PtMembersCount)
{
	typedef int(__thiscall* TPartyZero)(void*);
	TPartyZero PartyZero = (TPartyZero)0x007A7578;

	PartyZero(PtMembersCount);
	SetGameState(eGAME_STATE::GAME_STATE_PLAYING);
}

void __fastcall Discord::Hook::SetLobbyState(void* This, void* n, int Flag)
{
	typedef int(__thiscall* TSetLobby)(void*, int);
	TSetLobby SetLobby = (TSetLobby)0x00674BB1;

	SetLobby(This, Flag);
	SetGameState(static_cast<eGAME_STATE>(Game::LobbyType()));
}

void __fastcall Discord::Hook::SetGuild(void* pGuild, void* n, char* pGuildName, int Flag)
{
	typedef void(__thiscall* TSetGuildFunc)(void*, char*, int);
	TSetGuildFunc SetGuildFunc = (TSetGuildFunc)0x00502213;

	SetGuildFunc(pGuild, pGuildName, Flag);
	SetGameState(eGAME_STATE::GAME_STATE_PLAYING);
}

void __fastcall Discord::Hook::SetLFGAction(void* This, void* n, int a1, int a2, int a3)
{
	typedef void(__thiscall* TSetActionFunc)(void*, int, int, int);
	TSetActionFunc SetActionFunc = (TSetActionFunc)0x0093C1F0;

	SetGameState(eGAME_STATE::GAME_STATE_PLAYING);
	return SetActionFunc(This, a1, a2, a3);
}

void Discord::SetHooks()
{
	VM_STARTU(__FUNCTION__);
	using namespace Memory;
	HookFunc<HookType::CALL>(Hook::PartyInc, { 0x0062E14A, 0x00982F28, 0x00983E4A, 0x009840B9, 0x00A8E591 });
	HookFunc<HookType::CALL>(Hook::PartyDec, { 0x00982D60, 0x00983208, 0x00983772 });
	HookFunc<HookType::CALL>(Hook::PartyZero, { 0x0062E070, 0x007A687F, 0x00919270, 0x00982BCE, 0x00983CA2, 0x00983F09, 0x00A8E4C2 });
	HookFunc<HookType::CALL>(Hook::SetLobbyState, 0x007E8B94);
	HookFunc<HookType::CALL>(Hook::SetGuild, 0x00948800);
	HookFunc<HookType::CALL>(Hook::SetLFGAction, 0x006B6030);
	std::thread(Discord::CreateDiscord).detach();
	VM_END;
}