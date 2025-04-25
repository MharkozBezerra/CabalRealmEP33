#include "Base.h"
#include <windowsx.h>
#include "WndProc.h"
#include "Screen/Screen.h"
#include "GameDef.h"
#include "Basic/Basic.h"
#include "GameCustom/MyDraw/Objects.h"

POINT pRealCursor = { 0,0 };

typedef int(__thiscall* TSetGameCursor)(int, int, int);
TSetGameCursor SetGameCursor = (TSetGameCursor)0x00924BED;

LRESULT CALLBACK WindowProc::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	using namespace Screen;
	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_LBUTTONDOWN:
	{
		if (!MouseClickCustom(wParam, lParam))
			return 0;

		if (IsObjClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
			return 0;
		break;
	}
	case WM_MOUSEMOVE:
	{
		pRealCursor.x = GET_X_LPARAM(lParam);
		pRealCursor.y = GET_Y_LPARAM(lParam);

		float fRotate = 0.012f;

		if (!MouseMoveCustom(wParam, lParam))
			return 0;

		if (OnObjMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
			return (*reinterpret_cast<int*>(0x115AFA4) < 7) ? SetGameCursor(0x115AF90, eCabalCursorType::Normal, 0) : 0;

		break;
	}

	case WM_LBUTTONUP:
	{
		for (std::shared_ptr<sWindowObject>& pObj : DirectX::pWindowObjects)
		{
			if (pObj->Flag)
			{
				pObj->Flag = FALSE;
			}
		}
		break;
	}

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 'R':
			Basic::Game::Item::MoveItemToUi();
			break;
		case VK_F9:
		{
			if (!Screen::vLightMode.size())
				break;

			if (++Screen::LightMode >= Screen::vLightMode.size())
				Screen::LightMode = 0;

			std::string gameDir = Basic::Game::GetGameDir() + "\\Data\\UserData\\LightMode.dat";

			std::ofstream saveFile(gameDir);

			if (saveFile.is_open())
				saveFile.write(reinterpret_cast<char*>(&Screen::LightMode), sizeof(Screen::LightMode));

			saveFile.close();

			CabalDrawText((void*)0x00CFA638, Screen::vLightMode[Screen::LightMode].name.c_str(), green_screencenter, 1, -1);

			break;
		}

		case VK_ESCAPE:
		{
			for (std::shared_ptr<sWindowObject>& pObj : DirectX::pWindowObjects)
			{
				if (pObj->bCloseWKey && pObj->isVisible)
				{
					if (pObj->CloseWithKey())
						return 0;
					else break;
				}
			}
			break;
		}
		}
		break;
	case WM_DESTROY:
		SaveOnCloseCustom();
		break;
	}

	}
	return gameWndProc(hWnd, uMsg, wParam, lParam);
}

void WindowProc::InitHooks()
{
	VM_STARTU(__FUNCTION__);
	using namespace Memory;

	WriteValue<DWORD>(0x006539E1, (DWORD)WndProc);
	VM_END;
}