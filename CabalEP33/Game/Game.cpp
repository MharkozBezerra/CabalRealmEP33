#include "Base.h"
#include "Game.h"
#include "Memory/Memory.h"
#include "Discord/Discord.h"

void Game::MemoryChecks::InitMemoryChanges()
{
	VM_STARTU(__FUNCTION__);
	using namespace Memory;
#ifdef DEV_MODE
	//Xigncode
	SetMemory<BYTE>(0x009A1664, { 0xC2, 0x08, 0x00 });
	SetMemory<BYTE>(0x009A186E, { 0xC2, 0x0C, 0x00 });

	//Anti-Debugger
	Fill(0x00648E31, 0x90, 5);
#endif

	//XignCode Load Method
	WriteValue<BYTE>(0x009A1161, XIGNCODE_LOAD_METHOD);

	//CRC Memory Check
	SetMemory<BYTE>(0x0064448E, { 0xE9, 0x44, 0x01, 0x00, 0x00, 0x90 });

	//Remove __chdir
	Fill(0x0061708A, 0x90, 5);

	//Game data files are abnormal
	WriteValue<BYTE>(0x007E8CFA, 0xEB);

	//File not found or corrupted
	Fill(0x00666E79, 0x90, 2);
	Fill(0x00666E7D, 0x90, 2);

	//<&KERNEL32.CreateSemaphoreA> Multiple Games
	SetMemory<BYTE>(0x00642280, { 0x6A, MAX_GAME_INSTANCE, 0x6A, MAX_GAME_INSTANCE });

	//Language
	Fill(0x00605781, 0x90, 7);

	//File is corrupted
	SetMemory<BYTE>(0x00764310, { 0xC2, 0x04, 0x00 });

	// ViewEquipment
	Fill(0x0075A239, 0x90, 24);

	// Max.Chat.Len
	WriteValue<DWORD>(0x00867A44, 160);

	WriteValue<DWORD>(0x006404AA, (DWORD)GameWndName.c_str());
	VM_END;
}