#pragma once
#include "Base.h"
#include "Memory/Memory.h"
#include "DirectX/DirectX.h"

extern POINT pRealCursor;

namespace Screen
{
	int LoadGameFont(void* ths);
	BOOL IsMousePos(int pRealCursorPosX, int pRealCursorPosY, int x, int y, int w, int h);
	BOOL IsObjClick(int MouseX, int MouseY);
	BOOL OnObjMove(int MouseX, int MouseY);
	void RestoreUI(int ResX, int ResY);
	DWORD __fastcall SetLightMode(int* ths, void* n, int a2, int a3, int light_mode);
	void WINAPI OnLostDevice(LPCRITICAL_SECTION section);
	void InitHooks();
	namespace Draw
	{
		int __fastcall Login(void* ths);
		int __fastcall DrawAll(void* ths, void* n, int isHide);
	};
	
	//[world][dungeon][lightName]
	struct sLightMode
	{
		BYTE world;
		BYTE dungeon;
		BYTE newDungeon;
		BYTE war;
		std::string name;
	};

	void LoadData();
	extern std::vector<Screen::sLightMode> vLightMode;
	extern size_t LightMode;
}