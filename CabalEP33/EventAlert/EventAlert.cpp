#include "Base.h"
#include "EventAlert.h"
#include "Screen/Screen.h"
#include "DirectX/DirectX.h"
#include "Basic/Basic.h"
#include <format>

using namespace DirectX;
using namespace Screen;
using namespace Basic::Game;

void EventAlert::DrawItemOnSlot(EventAlert* This, std::shared_ptr<CGenericButton> pButton, int x, int y)
{
	auto &vTexture = pButton->GetTextures();

	if (vTexture.at(0))
	{
		if (auto& pSlot = This->pSlot)
			pSlot->DrawItemOnSlot(x + vTexture.at(0)->Rect.left, y + vTexture.at(0)->Rect.top, vTexture.at(0)->Rect.right, vTexture.at(0)->Rect.bottom, TRUE);
	}
}

EventAlert::EventAlert(int x, int y, int w, int h)
{
	VM_STARTV(__FUNCTION__);
	Scale		= TRUE;
	DinamicSave = FALSE;
	StaticObj	= TRUE;
	Flag		= FALSE;
	isVisible	= FALSE;
	bCloseWKey	= FALSE;
	ObjPos.X	= x;
	ObjPos.Y	= y;
	ObjPos.W	= w;
	ObjPos.H	= h;
	TexturesRct.push_back(pDirectX->LoadTextureWithRect(std::format("{}\\Data\\UserData\\EventAlert\\Bg.png", GetGameDir())));

	typedef sCabalWindow* (__thiscall* T_CALL)(void*, int);
	T_CALL WindowStatus = (T_CALL)0x00917F6C, ParentWindow = (T_CALL)0x007F85C7;
	pCabalWindow = WindowStatus(reinterpret_cast<void*>(0x115AF90), 40000000);

	pSlot = std::make_shared<cSlot>(eCustomSlot::CUSTOM_SLOT_EVENT_ALERT, eItemInfoType::E_INFO_TYPE_NORMAL, nullptr, pCabalWindow, TRUE);
	pSlot->SetInfoPadding(16, 0);

	mButtons.insert(std::pair<eTypeEventAlertButtons, std::vector<std::shared_ptr<CGenericButton>>>(eTypeEventAlertButtons::E_BUTTON_EVENT_ALERT_SLOT, {}));
	if (auto pButton = std::make_shared<CGenericButton>(eTypeEventAlertButtons::E_BUTTON_EVENT_ALERT_SLOT, (eTypeEventAlertButtons)0, std::format("{}\\Data\\UserData\\EventAlert\\Slot.png", GetGameDir()), std::format("{}\\Data\\UserData\\EventAlert\\Slot.png", GetGameDir()), 425, 64, FALSE))
	{
		pButton->SetDraw([this, pButton](int x, int y) { DrawItemOnSlot(this, pButton, x, y); });
		mButtons[E_BUTTON_EVENT_ALERT_SLOT].push_back(pButton);
	}

	REGISTERCUSTOMPROC(MAINCMD_VALUE_CUSTOM::S2C_EVENT_ALERT, &EventAlert::OnS2CEventAlert);
	VM_END;
}

void EventAlert::Draw()
{
	if (ScreenVisible())
	{
		if (USERDATACONTEXT* pUserDataCtx = USERDATACONTEXT::GetpUserDataCtx())
		{
			if (pUserDataCtx->GetOnLogged())
			{
				ObjPos.X = (static_cast<int>(MaxResolutionX / ObjectScaleX) - ObjPos.W) / 2;
				std::shared_ptr<EventQueue> pDrawEvent = nullptr;
				for (std::vector<std::shared_ptr<EventQueue>>::iterator it = vDrawEvent.begin(); it != vDrawEvent.end(); ++it)
				{
					if ((*it)->IsShowing())
					{
						pDrawEvent = *it;
						break;
					}
				}

				if (pDrawEvent)
				{
					pSlot->SetpItem(pDrawEvent->GetpItem());

					pDrawEvent->Draw(TexturesRct[0], ObjPos.X, ObjPos.Y);

					for (auto& pButton : mButtons[E_BUTTON_EVENT_ALERT_SLOT])
						pButton->Draw(ObjPos.X, ObjPos.Y);
				}
				else
				{
					pSlot->RemoveItemSlot(FALSE);
					vDrawEvent.clear();
				}
			}
		}
	}
}

BOOL EventAlert::OnHover(int x, int y)
{
	if (IsMousePos(x, y, (int)(ObjectScaleX * (ObjPos.X + 340)), (int)(ObjectScaleY * (ObjPos.Y)), (int)(ObjectScaleX * (216)), (int)(ObjectScaleY * (198))))
	{
		if (USERDATACONTEXT* pUserDataCtx = USERDATACONTEXT::GetpUserDataCtx())
		{
			if (!pUserDataCtx->getCursorItem())
				return TRUE;
		}
	}

	return FALSE;
}

BOOL EventAlert::ScreenVisible()
{
	return vDrawEvent.size() ? TRUE : FALSE;
}

void EventAlert::OnS2CEventAlert(S2C_HEADER_EX* pData, int size, void* unk)
{
	VM_STARTV(__FUNCTION__);
	typedef void(__thiscall* TDrawChatMsg)(void*, const char*, const char*, eChatColorType, sItemChatLink*, void*, int);
	TDrawChatMsg DrawChatMsg = (TDrawChatMsg)0x0072FD6A;

	typedef void* (__thiscall* TCharacterChatLink)(void*, int, char*);
	TCharacterChatLink CharacterChatLink = (TCharacterChatLink)0x00A22CE7;

	sS2C_EVENT_ALERT* pS2C_EVENT_ALERT = (sS2C_EVENT_ALERT*)pData;
	if (sItem* pItem = Item::GetpItem(pS2C_EVENT_ALERT->ItemId))
	{
		Item::TransformpItem(pItem, pS2C_EVENT_ALERT->ItemId, pS2C_EVENT_ALERT->ItemOpt);
		auto nameAndColor = Basic::Useful::getTextAndColor(Item::GetFullName(pItem, TRUE));

		if (!CheckTG(-1))
			pEventAlert->vDrawEvent.push_back(std::make_shared<EventQueue>(pItem, pS2C_EVENT_ALERT->CharName, Item::GetFullName(pItem), USERDATACONTEXT::getGameColorById(nameAndColor.second)));

		CabalDrawText(reinterpret_cast<void*>(0x00CFA638), std::vformat(pEventAlert->vTextList[pS2C_EVENT_ALERT->Event], std::make_format_args(std::string(pS2C_EVENT_ALERT->CharName), std::string(Item::GetFullName(pItem, TRUE)), GetMapName(nullptr, pS2C_EVENT_ALERT->MapId), GetChannelName(pS2C_EVENT_ALERT->Channel))).c_str(), system_only, 1, -1);
		
#ifdef EVENT_ALERT_DRAW_CHAT_MSG
		if (std::shared_ptr<sItemChatLink> pItemChatInfo = std::make_shared<sItemChatLink>())
		{
			if (USERDATACONTEXT* pUserDataCtx = USERDATACONTEXT::GetpUserDataCtx())
			{
				void* pCharacterChatInfo = nullptr;
				if (pUserDataCtx->GetCharacterIdx() != pS2C_EVENT_ALERT->CharIdx)
					pCharacterChatInfo = CharacterChatLink(reinterpret_cast<void*>(0x1188FC0), pS2C_EVENT_ALERT->CharIdx, pS2C_EVENT_ALERT->CharName);

				pItemChatInfo->ItemCount = 1;
				pItemChatInfo->ItemChatInfo.ListNum = 1;
				pItemChatInfo->ItemChatInfo.ItemIdx = pS2C_EVENT_ALERT->ItemId;
				pItemChatInfo->ItemChatInfo.ItemOpt = pS2C_EVENT_ALERT->ItemOpt;
				std::string str = std::vformat(pEventAlert->vTextList[pS2C_EVENT_ALERT->Event], std::make_format_args(pUserDataCtx->GetCharacterIdx() != pS2C_EVENT_ALERT->CharIdx ? std::format("$128#{}$", pS2C_EVENT_ALERT->CharName) : pS2C_EVENT_ALERT->CharName, std::format("$129#[{}]$", Item::GetCompleteName(pItem)), GetMapName(nullptr, pS2C_EVENT_ALERT->MapId), GetChannelName(pS2C_EVENT_ALERT->Channel))).c_str();
				DrawChatMsg(reinterpret_cast<void*>(0x00CFA638), str.c_str(), str.c_str(), ::E_CHAT_COLOR_NORMAL, pItemChatInfo.get(), pCharacterChatInfo, 0);
			}
		}
	}
#endif
	VM_END;
}

std::shared_ptr<EventAlert> pEventAlert;

EventQueue::EventQueue(sItem* _pItem, std::string _Owner, std::string _ItemName, D3DCOLOR _ItemColor, DWORD _dwMaxTime)
{
	pItem		= _pItem;
	Owner		= _Owner;
	ItemName	= _ItemName;
	ItemColor	= _ItemColor;
	dwMaxTime	= _dwMaxTime;
	dwWaitTime  = 0;
	dwEndTime	= 0;
	bShow		= TRUE;
}

void EventQueue::Draw(std::shared_ptr<DirectX::Texture::sTextureWithRect> TexturesRct, int x, int y)
{
	D3DXMATRIX oldMatrix, newMatrix;
	LPD3DXSPRITE pSprite = pDirectX->GetSprite();

	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	pSprite->GetTransform(&oldMatrix);
	pSprite->SetTransform(D3DXMatrixScaling(&newMatrix, ObjectScaleX, ObjectScaleY, 0));

	D3DXVECTOR3 ImgPos((float)x, (float)y, 0);
	
	pSprite->Draw(TexturesRct->lpTexture, nullptr, nullptr, &ImgPos, D3DCOLOR_ARGB(Opacity[0], 255, 255, 255));

	if (auto pButton = pEventAlert->mButtons[E_BUTTON_EVENT_ALERT_SLOT].back())
	{
		auto pTexture = pButton->GetTextures().at(0);
		D3DXVECTOR3 ImgPosSlot((float)(x + pTexture->Rect.left), (float)( y + pTexture->Rect.top), 0);

		pSprite->Draw(pTexture->lpTexture, nullptr, nullptr, &ImgPosSlot, D3DCOLOR_ARGB(Opacity[0], 255, 255, 255));
	}

	std::string OwnerInfo = std::format("{0} - Obtained", Owner.c_str());
	int OWidth = pDirectX->GetTextWidth(DirectX::Fonts::D3D9_FontsTypes::OVERLORD_LEVEL, OwnerInfo.c_str());
	RECT rct = { x + TexturesRct->Rect.left + ((TexturesRct->Rect.right - OWidth) / 2), y + 169 };
	pDirectX->DrawMessage(DirectX::Fonts::OVERLORD_LEVEL, &rct, D3DCOLOR_ARGB(Opacity[1], 0, 0, 0), OwnerInfo.c_str(), pSprite);
	rct.left++;
	rct.top++;
	pDirectX->DrawMessage(DirectX::Fonts::OVERLORD_LEVEL, &rct, D3DCOLOR_ARGB(Opacity[0], 181, 213, 240), OwnerInfo.c_str(), pSprite);

	int Width = pDirectX->GetTextWidth(DirectX::Fonts::OVERLORD_LEVEL, ItemName.c_str());
	RECT rctitem = { x + TexturesRct->Rect.left + ((TexturesRct->Rect.right - Width) / 2), y + 210 };
	pDirectX->DrawMessage(DirectX::Fonts::OVERLORD_LEVEL, &rctitem, D3DCOLOR_ARGB(Opacity[2], 0, 0, 0), ItemName.c_str(), pSprite);
	rctitem.left++;
	rctitem.top++;

	BYTE* pByteColor = (BYTE*)&ItemColor;
	pDirectX->DrawMessage(DirectX::Fonts::OVERLORD_LEVEL, &rctitem, D3DCOLOR_ARGB(Opacity[0], pByteColor[2], pByteColor[1], pByteColor[0]), ItemName.c_str(), pSprite);	
	
	pSprite->SetTransform(&oldMatrix);
	pSprite->End();

	DWORD dwGameTime = GetTickCount();
	if (dwWaitTime)
	{
		if (dwGameTime > dwWaitTime)
		{
			if (dwEndTime)
			{
				if (dwGameTime > dwEndTime)
				{
					bShow = FALSE;
					return;
				}
			}
			else dwEndTime = dwGameTime + dwMaxTime;

			DWORD dwElapsed = (dwEndTime - dwGameTime);
			Opacity[0] = (dwElapsed / (dwMaxTime / 255.f));
			Opacity[1] = (dwElapsed / (dwMaxTime / 80.f));
			Opacity[2] = (dwElapsed / (dwMaxTime / 200.f));
		}
	}
	else dwWaitTime = dwGameTime + 2000.0f;
}
