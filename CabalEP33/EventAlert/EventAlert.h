#pragma once
#include "Base.h"
#include "WindowObj/WindowObj.h"
#include "PacketManager/ProtosDef.h"
#include "GameDef.h"
#include "GenericButton/GenericButton.h"
#include "Slot/Slot.h"
#include "Proc/Proc.h"

enum eTypeEventAlert : BYTE
{
	E_INVALID_EVENT = 0,
	E_ITEM_DROP,
	E_ITEM_CREATE,
	E_MAX_EVENT
};

enum eTypeEventAlertButtons : int
{
	E_BUTTON_EVENT_ALERT_NONE,
	E_BUTTON_EVENT_ALERT_SLOT
};

enum eSubTypeEventAlertButtons : int
{
	E_SUB_EVENT_ALERT_NONE,
	E_SUB_EVENT_ALERT_SUB_SLOT,
};

#pragma pack( push, 1 )
struct sItemChatInfo
{
	DWORD ListNum;
	DWORD Unk;
	INT64 ItemIdx;
	INT64 ItemOpt;
	INT64 ItemDuration;
	INT64 ItemSerial;
	sItemChatInfo() : ListNum(0), Unk(0), ItemIdx(0), ItemOpt(0), ItemDuration(0), ItemSerial(0) {}
};

struct sItemChatLink
{
	BYTE Unk;
	BYTE ItemCount;
	sItemChatInfo ItemChatInfo;
	sItemChatLink() : Unk(0), ItemCount(0) {}
};
#pragma pack( pop )

struct sEventQueue
{
	sItem* pItem;
	std::string CharName;
	std::string ItemName;
};

class EventQueue
{
private:
	sItem* pItem;
	std::string Owner;
	std::string ItemName;
	BOOL bShow;
	DWORD dwWaitTime;
	DWORD dwMaxTime;
	DWORD dwEndTime;
	D3DCOLOR ItemColor;
	int Opacity[3] = { 255, 80, 200 };
public:
	EventQueue(sItem* _pItem, std::string _Owner, std::string _ItemName, D3DCOLOR _ItemColor = D3DCOLOR_ARGB(255, 254, 172, 44), DWORD _dwMaxTime = 2000.0f);
	BOOL IsShowing() { return bShow; };
	sItem* GetpItem() { return pItem; };
	void Draw(std::shared_ptr<DirectX::Texture::sTextureWithRect> TexturesRct, int x, int y);
};

class EventAlert : public sWindowObject
{
public:
	EventAlert(int x, int y, int w, int h);
	void Draw();
	void Save() {};
	BOOL CloseWithKey() { return FALSE; };
	BOOL ObjectClick(int x, int y) { return FALSE; };
	BOOL OnHover(int x, int y);
	BOOL ScreenVisible();

	std::map<int, std::vector<std::shared_ptr<CGenericButton>>> mButtons;
	std::shared_ptr<cSlot> pSlot;
	std::vector<std::shared_ptr<EventQueue>> vDrawEvent;
	std::vector<std::string> vTextList
	{
		{"Invalid Event Msg"},
		{"{} obtained {} on $39#{}$ - $34#{}$"},
		{"{} obtained {} opening $48#Box [DG \\ Event]$ on $39#{}$ - $34#{}$"}
	};

	static void DrawItemOnSlot(EventAlert* This, std::shared_ptr<CGenericButton> pButton, int x, int y);
	static void OnS2CEventAlert(S2C_HEADER_EX* pData, int size, void* unk);
	void ResetSlot()
	{
		if (pSlot)
			pSlot->SetReset();
	}

private:
	sCabalWindow* pCabalWindow = nullptr;
};

extern std::shared_ptr<EventAlert> pEventAlert;