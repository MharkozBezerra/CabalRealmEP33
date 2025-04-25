#include "Base.h"
#include "Screen.h"
#include "GameDef.h"
#include "BM3Sinergy/BM3Sinergy.h"
#include "EventAlert/EventAlert.h"
#include "../GameCustom/MyDraw/Objects.h"
#include "DLLSec/DLLSec.h"
#include "Basic/Basic.h"
#include "Discord/Discord.h"

using namespace DLLSec;

size_t Screen::LightMode = 0;
std::vector<Screen::sLightMode> Screen::vLightMode =
{
	{0,0,0,0,"Normal"}, //modo default obrigatório
	{1, 1, 1, 1, "Light Mode"},
	{10, 10, 10, 10, "Inverted Mode"},
	{4, 4, 4 , 4, "Dark Mode"}
};

int Screen::LoadGameFont(void* ths)
{
	VM_STARTV(__FUNCTION__);
	typedef int(__thiscall* t_LoadGameFont)(void* ths);
	t_LoadGameFont LoadGameFont_ = (t_LoadGameFont)0x0091E2B6;

	if (XignCode::CheckModules())
	{
		DirectX::InitDirectX();
		LoadCustomWindow();
		return LoadGameFont_(ths);
	}

	while (true);
	VM_END;
}

BOOL Screen::IsMousePos(int pRealCursorPosX, int pRealCursorPosY, int x, int y, int w, int h)
{
	return (pRealCursorPosX > x && pRealCursorPosX < x + w && pRealCursorPosY > y && pRealCursorPosY < y + h);
}

BOOL Screen::IsObjClick(int MouseX, int MouseY)
{
	for (std::shared_ptr<sWindowObject>& pObj : DirectX::pWindowObjects)
	{
		if (pObj->ScreenVisible())
		{
			if (pObj->ObjectClick(MouseX, MouseY)) return TRUE;

			if (!pObj->StaticObj && IsMousePos(MouseX, MouseY, (int)(ObjectScaleX * pObj->ObjPos.X), (int)(ObjectScaleY * pObj->ObjPos.Y), (int)(ObjectScaleX * pObj->ObjPos.W), (int)(ObjectScaleY * pObj->ObjPos.H)))
			{
				pObj->Flag = TRUE;
				pObj->ObjPos.sX = (int)(MouseX / ObjectScaleX) - pObj->ObjPos.X;
				pObj->ObjPos.sY = (int)(MouseY / ObjectScaleY) - pObj->ObjPos.Y;
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL Screen::OnObjMove(int MouseX, int MouseY)
{
	for (std::shared_ptr<sWindowObject>& pObj : DirectX::pWindowObjects)
	{
		if (pObj->ScreenVisible())
		{
			if (!pObj->StaticObj && pObj->Flag)
			{
				pObj->ObjPos.X = (int)(MouseX / ObjectScaleX) - pObj->ObjPos.sX;
				pObj->ObjPos.Y = (int)(MouseY / ObjectScaleY) - pObj->ObjPos.sY;

				if ((int)(pObj->ObjPos.X + pObj->ObjPos.W) * ObjectScaleX > MaxResolutionX)
				{
					pObj->ObjPos.X = (int)(MaxResolutionX / ObjectScaleX) - pObj->ObjPos.W;
					pObj->Flag = FALSE;
				}

				if ((int)(pObj->ObjPos.Y + pObj->ObjPos.H) * ObjectScaleY > MaxResolutionY)
				{
					pObj->ObjPos.Y = (int)(MaxResolutionY / ObjectScaleY) - pObj->ObjPos.H;
					pObj->Flag = FALSE;
				}

				if (pObj->ObjPos.X < 0)
				{
					pObj->ObjPos.X = 0;
					pObj->Flag = FALSE;
				}

				if (pObj->ObjPos.Y < 0)
				{
					pObj->ObjPos.Y = 0;
					pObj->Flag = FALSE;
				}
				return TRUE;
			}

			if (pObj->OnHover(MouseX, MouseY))
				return TRUE;
		}
	}
	return FALSE;
}

int __fastcall Screen::Draw::Login(void* ths)
{
	typedef int(__thiscall* t_Login_)(void*);
	t_Login_ Login_ = (t_Login_)0x005CC402;

	return Login_(ths);
}

int __fastcall Screen::Draw::DrawAll(void* ths, void* n, int isHide)
{
	typedef int(__thiscall* t_DrawAll)(void* ths, int isHide);
	t_DrawAll DrawAll_ = (t_DrawAll)0x009207E5;

	VM_STARTM(__FUNCTION__);
	if (USERDATACONTEXT* pUserDataCtx = USERDATACONTEXT::GetpUserDataCtx())
	{
		if (pUserDataCtx->GetOnLogged())
		{
			for (int i = 0, j = 0; i < 3; j = i, i++)
			{
				if (!((pSecAuth) && (pSecAuth->SecAuth[i] && pSecAuth->SecKey[i] && ((j) ? pSecAuth->xOR[i] == (((BYTE*)&pSecAuth->SecAuth[j])[0]) : pSecAuth->SecAuth[i] == (pSecAuth->SecKey[i] ^ pSecAuth->xOR[i]))) && ((pSecAuth->SecAuth[i] ^ ((j) ? (((BYTE*)&pSecAuth->SecAuth[j])[0]) : pSecAuth->xOR[i])) == pSecAuth->SecKey[i]) && (pSecAuth->Count[j] == pSecAuth->Count[i])))
					while (true);
			}
		}
	}

	if (!XignCode::CheckThreadList())
		while (true);
	VM_END;

	if (!isHide && pBM3Sinergy) pBM3Sinergy->Draw();

	DrawCustomWindowB(ths, isHide);

	auto value = DrawAll_(ths, isHide);

	DrawCustomWindowF(ths, isHide);

	if (!isHide && pEventAlert) pEventAlert->Draw();

	if (!isHide && pBM3Sinergy) pBM3Sinergy->DrawBuff();

	return value;
}

void Screen::RestoreUI(int ResX, int ResY)
{
	for (std::shared_ptr<sWindowObject>& pObj : DirectX::pWindowObjects)
	{
		if (((pObj->Scale) ? (int)(pObj->ObjPos.X + pObj->ObjPos.W) * ObjectScaleX : pObj->ObjPos.X + pObj->ObjPos.W) > ResX)
			pObj->ObjPos.X = (pObj->Scale) ? (int)(ResX / ObjectScaleX) - pObj->ObjPos.W : ResX - pObj->ObjPos.W;

		if (((pObj->Scale) ? (int)(pObj->ObjPos.Y + pObj->ObjPos.H) * ObjectScaleY : pObj->ObjPos.Y + pObj->ObjPos.H) > ResY)
			pObj->ObjPos.Y = (pObj->Scale) ? (int)(ResY / ObjectScaleY) - pObj->ObjPos.H : ResY - pObj->ObjPos.H;

		if (pObj->ObjPos.X < 0)
			pObj->ObjPos.X = 0;

		if (pObj->ObjPos.Y < 0)
			pObj->ObjPos.Y = 0;
	}
}

DWORD __fastcall Screen::SetLightMode(int* ths, void* n, int a2, int a3, int light_mode)
{
	typedef DWORD(__thiscall* t_SetLight)(int* ths, int a2, int a3, int light_mode);
	t_SetLight SetLight = (t_SetLight)0x0065B6CF;

	auto pUserDataCtx = USERDATACONTEXT::GetpUserDataCtx();

	if (pUserDataCtx && LightMode)
	{
		int dungeonIdx = pUserDataCtx->GetDungeonIdx();

		if (dungeonIdx)
		{
			light_mode = (dungeonIdx >= 4263) ? Screen::vLightMode[LightMode].newDungeon : Screen::vLightMode[LightMode].dungeon;
		}
		else
		{
			light_mode = CheckTG(pUserDataCtx->GetMapId()) ? Screen::vLightMode[LightMode].war : Screen::vLightMode[LightMode].world;
		}
	}

	return SetLight(ths, a2, a3, light_mode);
}

void Screen::LoadData()
{
	VM_STARTU(__FUNCTION__);
	std::string gameDir = Basic::Game::GetGameDir() + "\\Data\\UserData\\LightMode.dat";

	std::ifstream openFile(gameDir);

	if (openFile.is_open())
		openFile.read(reinterpret_cast<char*>(&Screen::LightMode), sizeof(Screen::LightMode));

	openFile.close();
	VM_END;
}

void WINAPI Screen::OnLostDevice(LPCRITICAL_SECTION section)
{
	if (DirectX::pDirectX)
	{
		DirectX::pDirectX->OnLostAllFonts();
		DirectX::pDirectX->OnResetAllFonts();

		for (std::shared_ptr<sWindowObject>& pObj : DirectX::pWindowObjects)
			pObj->ResetSlot();
	}

	OnLostDeviceCustom();

	LeaveCriticalSection(section);
}

void Screen::InitHooks()
{
	VM_STARTU(__FUNCTION__);
	using namespace Memory;
	LoadData();
	HookFunc<HookType::CALL>(LoadGameFont, 0x0091DEB5);
	HookFunc<HookType::CALL>(Draw::Login, 0x007EB0BE);
	HookFunc<HookType::CALL>(Draw::DrawAll, { 0x007EB003, 0x007F49A2 });
	HookFunc<HookType::CALL>(SetLightMode, { 0x00687E81, 0x007CBE6A });
	HookFunc<HookType::CALL_LONG>(OnLostDevice, 0x006551CD);
	VM_END;
}