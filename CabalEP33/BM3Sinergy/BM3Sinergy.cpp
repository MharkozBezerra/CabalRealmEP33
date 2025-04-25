#include "Base.h"
#include "BM3Sinergy.h"
#include <nlohmann/json.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "DirectX/DirectX.h"
#include "Screen/Screen.h"
#include "Basic/Basic.h"

using namespace DirectX;
using namespace Screen;
using namespace Basic::Game;
BM3Sinergy::BM3Sinergy(int x, int y, int w, int h)
{
	VM_STARTV(__FUNCTION__);
	using namespace Memory;
	HookFunc<HookType::JMP>(SinergyProc, 0x009CDFCE);
	HookFunc<HookType::CALL>(SinergyRemove, 0x006A2452);

	Scale			= TRUE;
	DinamicSave		= FALSE;
	StaticObj		= TRUE;
	Flag			= FALSE;
	isVisible		= FALSE;
	bCloseWKey		= FALSE;
	ObjPos.X		= x;
	ObjPos.Y		= y;
	ObjPos.W		= w;
	ObjPos.H		= h;

	Textures.push_back(pDirectX->LoadTexture(std::format("{}\\Data\\UserData\\Sinergy\\Bg.png", GetGameDir())));
	Textures.push_back(pDirectX->LoadTexture(std::format("{}\\Data\\UserData\\Sinergy\\Button.png", GetGameDir())));

	for (int i = 1; i <= 9; i++)
		TexturesRct.push_back(pDirectX->LoadTextureWithRect(std::format("{}\\Data\\UserData\\Sinergy\\BMIcons\\{}.png", GetGameDir(), i)));

	LoadJsonInfos();
	VM_END;
}

void BM3Sinergy::LoadJsonInfos()
{
	VM_STARTV(__FUNCTION__);
	std::ifstream f(std::format("{}\\Data\\UserData\\Sinergy\\Custom.json", GetGameDir()));
	if (f.is_open())
	{
		nlohmann::json data = nlohmann::json::parse(f);
		nlohmann::json sinergy = data["battlemode3"];
		nlohmann::json buffs = sinergy["bm3_synergy"];

		for (int ClassId = eClassCode::GU; ClassId <= eClassCode::DM; ClassId++)
		{
			mSinergyList.insert(std::pair<int, std::vector<sSinergyList>>(ClassId, {}));
			if (sinergy.contains(std::to_string(ClassId)))
			{
				for (const auto& result : sinergy[std::to_string(ClassId)])
				{
					sSinergyList SinergyList;
					SinergyList.Id = result["id"].get<int>();
					SinergyList.Combo = result["combo"].get<std::string>();

					if (buffs.contains(std::to_string(SinergyList.Id)))
					{
						for (const auto& result2 : buffs[std::to_string(SinergyList.Id)])
						{
							sSinergyBuff SinergyBuff;
							SinergyBuff.force_code = result2["force_code"].get<int>();
							SinergyBuff.force_value = result2["force_value"].get<int>();
							SinergyBuff.force_type = result2["value_type"].get<int>();
							SinergyBuff.target_type = result2["target_type"].get<int>();
							SinergyList.vBuffList.push_back(SinergyBuff);
						}
					}

					mSinergyList[ClassId].push_back(SinergyList);
				}
			}
		}
	}
	VM_END;
}

void BM3Sinergy::Draw()
{
	if (ScreenVisible())
	{
		pDirectX->DrawSprite(Textures[0], D3DXSPRITE_ALPHABLEND, (float)ObjPos.X, (float)ObjPos.Y);
		if (int SinergyProcCount = USERDATACONTEXT::GetSinergyProcCount())
		{
			for (int i = 0; i < SinergyProcCount; i++)
			{
				pDirectX->DrawSprite(Textures[1], D3DXSPRITE_ALPHABLEND, (float)ObjPos.X + (i * 12), (float)ObjPos.Y);
			}
		}
	}
}

void BM3Sinergy::DrawBuff()
{
	if (ScreenVisible())
	{
		if (IsMousePos(pRealCursor.x, pRealCursor.y, (int)(ObjectScaleX * (ObjPos.X)), (int)(ObjectScaleY * (ObjPos.Y)), (int)(ObjectScaleX * ObjPos.W), (int)(ObjectScaleY * ObjPos.H)))
		{
			if (pDescription)
				pDescription->Draw(ObjPos.X + 35, ObjPos.Y + 6);
		}
	}
}

void BM3Sinergy::SetPlayerSinergy()
{
	VM_STARTV(__FUNCTION__);
	typedef sBattleMode3* (__thiscall* TGetSinergyPtr)(void*, int);
	TGetSinergyPtr GetSinergyPtr = (TGetSinergyPtr)0x006BE862;

	typedef char* (__cdecl* TGetSinergyName)(DWORD, char*);
	TGetSinergyName GetSinergyName = (TGetSinergyName)0x00722EFE;

	typedef char* (__thiscall* TLoadFileMsgById)(void*, int);
	TLoadFileMsgById LoadFileMsgById = (TLoadFileMsgById)0x007343E2;

	typedef char* (__thiscall* TMapName)(int*);
	TMapName ObjName = (TMapName)0x0045E520;

	if (vPlayerSinergy.size())
		vPlayerSinergy.clear();

	if (USERDATACONTEXT* pUserDataCtx = USERDATACONTEXT::GetpUserDataCtx())
	{
		auto it = mSinergyList.find(pUserDataCtx->GetCharacterClass());
		if (it != mSinergyList.end())
		{
			if (pDescription = std::make_shared<cDescription>("Sinergy Buff", D3DCOLOR_ARGB(255, 254, 172, 44), 289, FALSE, 15, 35))
			{
				int iClassCode = pUserDataCtx->GetCharacterClass() - 1;
				if (TexturesRct.size() >= iClassCode)
				{
					RECT rctIcon = { 15, 14, 38, 38 };
					pDescription->SetIcon(TexturesRct[iClassCode]->lpTexture, &rctIcon);
				}

				for (auto& m : it->second)
				{
					if (sBattleMode3* pSinergy = GetSinergyPtr(reinterpret_cast<void*>(0xCECE88), m.Id))
					{
						if (std::shared_ptr<sPlayerSinergy> pPlayerSinergy = std::make_shared<sPlayerSinergy>())
						{

							pPlayerSinergy->Name = ObjName(reinterpret_cast<int*>(GetSinergyName(0xCFA020, pSinergy->NamePrefix)));
							pPlayerSinergy->Combo = m.Combo;
							pPlayerSinergy->Id = m.Id;
							pPlayerSinergy->Level = 1;
							pPlayerSinergy->Proc = FALSE;

							if (vPlayerSinergy.size())
							{
								if (auto pMsg = std::make_shared<cMsgWithSeparator>("", D3DCOLOR_ARGB(255, 254, 91, 44), FALSE, TRUE))
									pDescription->SetBodyStringShort(pMsg);
							}

							if (auto pMsg = std::make_shared<cMsgWithSeparator>(std::format("{} ({})", pPlayerSinergy->Name, pPlayerSinergy->Combo), D3DCOLOR_ARGB(255, 254, 91, 44), TRUE, TRUE, 5, 5))
							{
								pDescription->SetBodyStringShort(pMsg);
							}

							for (auto b : m.vBuffList)
							{
								if (auto pMsg = std::make_shared<cMsgWithSeparator>(std::format("{} {}{}{}", LoadFileMsgById(reinterpret_cast<void*>(0xCFA638), mForceList.at(b.force_code)), (b.force_value > 0) ? "+" : "", b.force_value, (b.force_type > 1) ? "%" : ""), D3DCOLOR_ARGB(255, 255, 255, 255)))
									pDescription->PushMiltiInfoString(pMsg, std::format("Trigger: {}", (b.target_type == 1) ? "Self" : "Opponent"), D3DCOLOR_XRGB(165, 165, 165), D3DCOLOR_XRGB(75, 240, 255), &pPlayerSinergy->Proc, TRUE, TRUE);
							}

							vPlayerSinergy.push_back(pPlayerSinergy);
						}
					}
				}
			}
		}
	}
	VM_END;
}

BOOL BM3Sinergy::ScreenVisible()
{
	typedef sCabalWindow* (__thiscall* T_CALL)(void*, int);
	T_CALL WindowStatus = (T_CALL)0x00917F6C, ParentWindow = (T_CALL)0x007F85C7;

	if (USERDATACONTEXT* pUserData = USERDATACONTEXT::GetpUserDataCtx())
	{
		if (pUserData->GetBattleType()[0] & BattleModeType::BM3)
		{
			std::pair<int, int> WndInfos = pUserData->GetBattleModeWindowId();
			if (pCabalWindow = WindowStatus(reinterpret_cast<void*>(0x115AF90), WndInfos.first))
			{
				if (pSecondWindow = ParentWindow(reinterpret_cast<void*>(pCabalWindow), WndInfos.second))
				{
					ObjPos.Y = (pSecondWindow->GetObjectbY() / ObjectScaleY);
					return pSecondWindow->GetWindowStat();
				}
			}
		}
	}

	return FALSE;
}

void __fastcall BM3Sinergy::SinergyProc(sSinergyProcInfo* pSinergyInfo)
{
	typedef void(__thiscall* TSinergyFunc)(sSinergyProcInfo*);
	TSinergyFunc SinergyFunc = (TSinergyFunc)0x009D0844;

	if (pBM3Sinergy)
	{
		auto it = std::find_if(pBM3Sinergy->vPlayerSinergy.begin(), pBM3Sinergy->vPlayerSinergy.end(), [pSinergyInfo](std::shared_ptr<sPlayerSinergy> p) {return (p->Id == pSinergyInfo->Id); });
		if (it != pBM3Sinergy->vPlayerSinergy.end())
		{
			(*it)->Level = pSinergyInfo->Level;
			(*it)->Proc  = TRUE;
		}
	}

	SinergyFunc(pSinergyInfo);
}

BYTE __fastcall BM3Sinergy::SinergyRemove(void* This, void* n, int SinergyId)
{
	typedef BYTE(__thiscall* TSinergyRemoveFunc)(void*, int);
	TSinergyRemoveFunc SinergyRemoveFunc = (TSinergyRemoveFunc)0x006C695A;

	if (pBM3Sinergy)
	{
		auto it = std::find_if(pBM3Sinergy->vPlayerSinergy.begin(), pBM3Sinergy->vPlayerSinergy.end(), [SinergyId](std::shared_ptr<sPlayerSinergy> p) {return (p->Id == SinergyId); });
		if (it != pBM3Sinergy->vPlayerSinergy.end())
		{
			(*it)->Level = 1;
			(*it)->Proc = FALSE;
		}
	}

	return SinergyRemoveFunc(This, SinergyId);
}

std::shared_ptr<BM3Sinergy> pBM3Sinergy = nullptr;