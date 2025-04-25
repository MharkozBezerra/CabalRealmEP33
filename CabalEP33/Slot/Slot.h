#pragma once
#include "Base.h"
#include "GameDef.h"
#include <functional>

enum eItemInfoType
{
	E_INFO_TYPE_NORMAL = -1,
};

enum eCustomSlot
{
	CUSTOM_SLOT_INVALID = 0,
	CUSTOM_SLOT_EVENT_ALERT,
};

class cSlot
{
public:
	cSlot(int _SlotId, int _InfoType, std::function<BOOL(sItem*, int)> _fnItemCheck = nullptr, sCabalWindow* _pWnd = nullptr, BOOL _bNoLink = FALSE);

	void DrawItemOnSlot(int x, int y, int w, int h, BOOL bHover);
	void SetInfoPadding(int x, int y);
	void __declspec(noinline) DrawItemInfo(int x, int y, int w, int h, BOOL bHover);
	void BindRemoveSlot(std::function<void()> fRemove, bool beforeRemoveItem = false);
	void BindInsertSlot(std::function<void()> fRemove, bool beforeRemoveItem = false);
	void SetItemSlot(USERDATACONTEXT* pUserDataCtx_ = nullptr);
	void SetItemSlot(sItemSlot& _ItemSlot, USERDATACONTEXT* pUserDataCtx_ = nullptr);
	void RemoveItemSlot(BOOL bUnlock = FALSE);
	void LockItemSlot(USERDATACONTEXT* pUserDataCtx, sItem* _pItem);
	void LockItemSlot(sItem* _pItem, sItemSlot& ItemSlot_);
	void UnbindItemOnMouse(USERDATACONTEXT* pUserDataCtx);
	void BindItemOnMouse(USERDATACONTEXT* pUserDataCtx);
	void SetpItem(int ItemId);
	void SetpItem(sItem* _pItem) { pItem = _pItem; };
	void SetInfoType(int _InfoType) { InfoType = _InfoType; };
	void SetScale(float Scale) { ItemScale = Scale; };
	void SetpWnd(sCabalWindow* pWnd_) { pWnd = pWnd_; };
	void SetbLocked(BOOL _bLocked) { bLocked = _bLocked; };
	void SetbNoLink(BOOL _bNoLink) { bNoLink = _bNoLink; };
	void CreateItemInfo(BOOL bSetVisible);

	void SetReset()
	{
		bReset = TRUE;
	}

	sItem* GetpItem() { return pItem; };
	int GetSlot() { return ItemSlot; };
	eItemOrigin GetType() { return Type; };
	float GetScale() { return ItemScale; };
	sCabalWindow* GetpWnd() { return pWnd; };
	BOOL GetbLocked() { return bLocked; };
	BOOL GetbNoLink() { return bNoLink; };
	static sItem* __fastcall GetpItemInfo(sCabalWindow* _pWnd);
private:
	std::list<std::pair<bool, std::function<void()>>> listRemoveCallback;
	std::list<std::pair<bool, std::function<void()>>> listInsertCallback;
	int SlotId;
	int InfoType = -1;
	int paddingX = 0;
	int paddingY = 0;
	sItem* pItem;
	int ItemSlot;
	eItemOrigin Type;
	int Unk;
	sCabalWindow* pWnd;
	BOOL bLocked = FALSE;
	BOOL bReset = FALSE;
	BOOL bNoLink = FALSE;
	float ItemScale = 1.0f;
	std::function<BOOL(sItem*, int)> fnItemCheck;
};

extern std::vector<cSlot*> vpSlot;