#include "Base.h"
#include "Slot.h"
#include "Basic/Basic.h"
#include <format>
#include "DirectX/DirectX.h"
#include "Memory/Memory.h"

using namespace Basic;
using namespace DirectX;

std::vector<cSlot*> vpSlot;

cSlot::cSlot(int _SlotId, int _InfoType, std::function<BOOL(sItem*, int)> _fnItemCheck, sCabalWindow* _pWnd, BOOL _bNoLink)
{
	SlotId = _SlotId; 
	InfoType = _InfoType;
	fnItemCheck = _fnItemCheck; 
	pWnd = _pWnd;
	pItem = nullptr;
	ItemSlot = -1;
	Type = eItemOrigin::e_ORIGIN_INVALID;
	Unk = -1;
	bNoLink = _bNoLink;
	vpSlot.push_back(this);
}

void cSlot::DrawItemOnSlot(int x, int y, int w, int h, BOOL bHover)
{
	typedef void(__stdcall* TPrintItemTexture)(sItem*, float, float, float, float, int, int, int, int);
	TPrintItemTexture PrintItemTexture = (TPrintItemTexture)0x00A4A96C;

	typedef double(__cdecl* TItemScaling)(sItem*);
	TItemScaling ItemScaling = (TItemScaling)0x00A44710;

	typedef HRESULT(__stdcall* TCustomSetTransform)(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix);
	TCustomSetTransform CustomSetTransform = (TCustomSetTransform)0x0065B70B;

	if (USERDATACONTEXT* pUserDataCtx = USERDATACONTEXT::GetpUserDataCtx())
	{
		if (pItem && pUserDataCtx->GetOnLogged())
		{
			D3DXMATRIX* pOutOrthoLH = (D3DXMATRIX*)&((char*)pItem)[0x90];
			D3DXMATRIX* pOutLookAtLH = (D3DXMATRIX*)&((char*)pItem)[0xD0];

			if ((!pOutOrthoLH) || (!pOutLookAtLH))
				return;

			float* zValue = reinterpret_cast<float*>(0x00BB38A4);
			float mX = static_cast<float>(MaxResolutionX);
			float mY = static_cast<float>(MaxResolutionY);
			D3DXMatrixOrthoLH(pOutOrthoLH, mX, mY, 0, 5000.0f);

			D3DXVECTOR3 vList[3] = { {mX * *zValue, mY * *zValue, -1000.f}, {mX * *zValue, mY * *zValue, 0}, {0, 1.0f, 0} };
			D3DXMatrixLookAtLH(pOutLookAtLH, &vList[0], &vList[1], &vList[2]);

			CustomSetTransform(D3DTS_PROJECTION, pOutOrthoLH);
			CustomSetTransform(D3DTS_VIEW, pOutLookAtLH);
			float fAnimTime = bHover ? (float)(*reinterpret_cast<double*>(*reinterpret_cast<DWORD*>(0x00CE7EAC) + 0x3C8) * *reinterpret_cast<double*>(0x00BB35A0)) : 0.0f; //atualizado			
			PrintItemTexture(pItem, (ObjectScaleX * (float)(x + (w / 2))), (ObjectScaleY * (float)(y + (h / 2))), (float)ItemScaling(pItem), fAnimTime, 0, 0, 0, -1);
		}

	}
}

void cSlot::SetInfoPadding(int x, int y)
{
	paddingX = x;
	paddingY = y;
}

void cSlot::DrawItemInfo(int x, int y, int w, int h, BOOL bHover)
{
	if (USERDATACONTEXT* pUserDataCtx = USERDATACONTEXT::GetpUserDataCtx())
	{
		/*if (pWnd && !pUserDataCtx->getCursorItem())
		{
			bool equalItem = pWnd->pItem == pItem;
			if (bHover)
			{
				if (!equalItem || bReset)
				{
					bReset = FALSE;

					pWnd->pItem = pItem;
					pWnd->SlotHover = ItemSlot;
					pWnd->TypeInfo = InfoType;
					pWnd->rX = (ObjectScaleX * (float)(x + (w))) + paddingX;
					pWnd->rY = (ObjectScaleY * (float)(y + paddingY));
					CreateItemInfo(TRUE);
				}
			}
			else
			{
				if (equalItem)
				{
					pWnd->pItem = nullptr;
					CreateItemInfo(FALSE);
				}
			}
		}*/
	}
}

void cSlot::BindRemoveSlot(std::function<void()> fRemove, bool beforeRemoveItem)
{
	listRemoveCallback.push_back(std::make_pair(beforeRemoveItem, fRemove));
}

void cSlot::BindInsertSlot(std::function<void()> fRemove, bool beforeRemoveItem)
{
	listInsertCallback.push_back(std::make_pair(beforeRemoveItem, fRemove));
}

void cSlot::SetItemSlot(USERDATACONTEXT* pUserDataCtx_)
{
	typedef BOOL(__cdecl* TCtrlPressed)();
	TCtrlPressed CtrlPressed = (TCtrlPressed)0x0042B030;

	USERDATACONTEXT* pUserDataCtx = pUserDataCtx_ ? pUserDataCtx_ : USERDATACONTEXT::GetpUserDataCtx();
	if ((pUserDataCtx) && (!bLocked))
	{
		if (sItem* _pItem = pUserDataCtx->getCursorItem())
		{
			if (pUserDataCtx->GetItemClick()->Type == eItemOrigin::e_ORIGIN_INVENTORY)
			{				
				if ((fnItemCheck) && (!fnItemCheck(_pItem, SlotId)))
					return CabalDrawText(reinterpret_cast<void*>(0x00CFA638), std::format("{0} não pode ser usado.", Game::Item::GetCompleteName(_pItem)).c_str(), green_screencenter, 1, -1);
					
				if (pItem)
					RemoveItemSlot(TRUE);

				for (auto& function : listInsertCallback)
					if (function.first)
						function.second();

				LockItemSlot(pUserDataCtx, _pItem);

				for (auto& function : listInsertCallback)
					if (!function.first)
						function.second();
			}
			else CabalDrawText(reinterpret_cast<void*>(0x00CFA638), "Só é possível usar items do inventário do personagem.",  green_screencenter, 1, -1);
		} 
		else 
		if (pItem)
			CtrlPressed() ? RemoveItemSlot(TRUE) : BindItemOnMouse(pUserDataCtx);
	}
}

void cSlot::SetItemSlot(sItemSlot& _ItemSlot, USERDATACONTEXT* pUserDataCtx_)
{
	USERDATACONTEXT* pUserDataCtx = pUserDataCtx_ ? pUserDataCtx_ : USERDATACONTEXT::GetpUserDataCtx();
	if ((pUserDataCtx) && (!bLocked))
	{
		if ((_ItemSlot.Type == eItemOrigin::e_ORIGIN_INVENTORY) && (_ItemSlot.Slot >= E_START_INV_SLOT))
		{
			if (sItem* _pItem = pUserDataCtx->GetpSlotItem(_ItemSlot.Slot))
			{
				if (_pItem->bUnlocked)
				{
					if ((fnItemCheck) && (!fnItemCheck(_pItem, SlotId)))
						return CabalDrawText(reinterpret_cast<void*>(0x00CFA638), std::format("{0} não pode ser usado.", Game::Item::GetCompleteName(_pItem)).c_str(), green_screencenter, 1, -1);

					if (pItem)
						RemoveItemSlot(TRUE);

					for (auto& function : listInsertCallback)
						if (function.first)
							function.second();

					LockItemSlot(_pItem, _ItemSlot);

					for (auto& function : listInsertCallback)
						if (!function.first)
							function.second();
				}
			}
		}
		else CabalDrawText(reinterpret_cast<void*>(0x00CFA638), "Só é possível usar items do inventário do personagem.", green_screencenter, 1, -1);
	}
}

void cSlot::RemoveItemSlot(BOOL bUnlock)
{
	if ((bUnlock) && (!bLocked) && (pItem))
		pItem->bUnlocked = TRUE;

	for (auto& function : listRemoveCallback)
		if (function.first)
			function.second();

	pItem = nullptr;
	ItemSlot = -1;
	Type = eItemOrigin::e_ORIGIN_INVALID;
	Unk = -1;
	bLocked = FALSE;

	/*if (pWnd)
	{
		pWnd->pItem = nullptr;
		pWnd->SlotHover = -1;
		CreateItemInfo(FALSE);
	}*/

	for (auto& function : listRemoveCallback)
		if (!function.first)
			function.second();
}

void cSlot::LockItemSlot(USERDATACONTEXT* pUserDataCtx, sItem* _pItem)
{
	if (pItem)
		pItem->bUnlocked = TRUE;

	pItem				= _pItem;
	ItemSlot			= pUserDataCtx->GetItemClick()->ClickedSlot;
	Type				= pUserDataCtx->GetItemClick()->Type;
	Unk					= pUserDataCtx->GetItemClick()->Unk;
	pItem->bUnlocked	= FALSE;
	UnbindItemOnMouse(pUserDataCtx);
}

void cSlot::LockItemSlot(sItem* _pItem, sItemSlot& ItemSlot_)
{
	if (pItem)
		pItem->bUnlocked = TRUE;

	pItem    = _pItem;
	ItemSlot = ItemSlot_.Slot;
	Type     = static_cast<eItemOrigin>(ItemSlot_.Type);
	Unk      = 0;
	pItem->bUnlocked = FALSE;
}

void cSlot::UnbindItemOnMouse(USERDATACONTEXT* pUserDataCtx)
{
	pUserDataCtx->GetItemClick()->Type			= eItemOrigin::e_ORIGIN_INVALID;
	pUserDataCtx->GetItemClick()->ClickedSlot	= 0;
	pUserDataCtx->GetItemClick()->Unk			= -1;
	pUserDataCtx->SetpSlotItem(0, E_MOUSE);
}

void cSlot::BindItemOnMouse(USERDATACONTEXT* pUserDataCtx)
{
	pUserDataCtx->SetpSlotItem(pItem, E_MOUSE);
	pUserDataCtx->GetItemClick()->ClickedSlot	= ItemSlot;
	pUserDataCtx->GetItemClick()->Type			= Type;
	pUserDataCtx->GetItemClick()->Unk			= Unk;
	RemoveItemSlot();
}

void cSlot::SetpItem(int ItemId)
{
	pItem = Game::Item::GetpItem(ItemId);
}

void cSlot::CreateItemInfo(BOOL bSetVisible)
{
	typedef void(__thiscall* TCreateUI)(sCabalWindow*);
	typedef void(__thiscall* TSetWindow)(sCabalWindow*, BOOL);
	TCreateUI CreateUI = (TCreateUI)0x0040D2E2;
	TSetWindow SetWindow = (TSetWindow)0x0043CA2E;

	if (bSetVisible)
		CreateUI(pWnd);

	SetWindow(pWnd, bSetVisible);
}

sItem* __fastcall cSlot::GetpItemInfo(sCabalWindow* _pWnd)
{
	typedef sItem* (__thiscall* TOriFunc)(sCabalWindow*);
	TOriFunc OriFunc = (TOriFunc)0x006A460E;

	/*if (_pWnd->TypeInfo == eItemInfoType::E_INFO_TYPE_NORMAL)
	{
		for (auto& pSlot : vpSlot)
		{
			if ((pSlot) && (pSlot->GetbNoLink()))
			{
				if (sItem* pItem = pSlot->GetpItem())
				{
					if (pItem == _pWnd->pItem)
						return pItem;
				}
			}
		}
	}*/

	return OriFunc(_pWnd);
}
