#pragma once
#include "Base.h"
#include "DirectX/DirectX.h"

using namespace DirectX;

class cProgressBar
{
public:
	std::shared_ptr<Texture::sTextureWithRect> lpTextureBar;
	std::shared_ptr<Texture::sTextureWithRect> lpTextureBar_;

	cProgressBar(std::string Dir, std::string Dir_)
	{
		lpTextureBar =  pDirectX->LoadTextureWithRect(Dir);
		lpTextureBar_ = pDirectX->LoadTextureWithRect(Dir_);
	}
};

struct sDescriptionRequerimentInfo32
{
	std::vector<int> vRequeriments;
	int* pTriggerPosition;
	int* pTriggerRequeriment;

	sDescriptionRequerimentInfo32()
	{
		pTriggerPosition = nullptr;
		pTriggerRequeriment = nullptr;
	}
};

class cMsgWithSeparator
{
public:
	std::string strBody;
	BOOL bUseSeparator;
	RECT rctMsg{};
	RECT rctSeparator{};
	D3DCOLOR color;
	int PaddingTop, PaddingBottom;
	BOOL bCenter;

	cMsgWithSeparator(std::string strBody_, D3DCOLOR color_, BOOL bCenter_ = FALSE, BOOL bSeparator = FALSE, int PaddingTop_ = 0, int PaddingBottom_ = 0) : strBody(strBody_), color(color_), bUseSeparator(bSeparator), PaddingTop(PaddingTop_), PaddingBottom(PaddingBottom_), bCenter(bCenter_)
	{

	};
};

struct sDescriptionMultiInfo32
{
	int* pTrigger;
	std::vector<int> vValues;
	D3DCOLOR ColorValueOff;
	D3DCOLOR ColorValueOn;
	BOOL bUseTrigger;
	BOOL bUseTriggerWithPos;
	BOOL bUsePercent;
	std::shared_ptr<cMsgWithSeparator> pMessage;
	std::shared_ptr<sDescriptionRequerimentInfo32> pRequeriments;

	sDescriptionMultiInfo32()
	{
		pMessage = nullptr;
		pTrigger = nullptr;
		pRequeriments = nullptr;
		bUseTrigger = FALSE;
		bUseTriggerWithPos = FALSE;
		bUsePercent = FALSE;
	}
};

struct sDescriptionMultiInfoString
{
	int* pTrigger;
	std::string sValue;
	D3DCOLOR ColorValueOff;
	D3DCOLOR ColorValueOn;
	BOOL bUseTrigger;
	BOOL bUseTriggerWithBool;
	std::shared_ptr<cMsgWithSeparator> pMessage;

	sDescriptionMultiInfoString()
	{
		pMessage = nullptr;
		pTrigger = nullptr;
		bUseTrigger = FALSE;
		bUseTriggerWithBool = FALSE;
	}
};

class cDescription
{
	std::string strTitle;
	D3DCOLOR colorTitle;
	RECT rct;
	RECT rctIcon;

	int* pTriggerCur;
	int* pTriggerCnt;
	int* pFinish;
	int tX, tY, PaddingTop, PaddingBottom;
	int SeparatorWidth;

	std::vector<std::shared_ptr<sDescriptionMultiInfo32>> vMultiInfosMessage;
	std::vector<std::shared_ptr<sDescriptionMultiInfoString>> vMultiInfosString;

	std::vector<std::shared_ptr<cMsgWithSeparator>> vBodyMsg;
	std::shared_ptr<cMsgWithSeparator> pMsgProgress;
	std::shared_ptr<cMsgWithSeparator> pLastStr;
	std::shared_ptr<cProgressBar> pProgressBar;
	LPDIRECT3DTEXTURE9 lpTexture[12]{};
	std::vector<std::pair<LPDIRECT3DTEXTURE9, RECT>> vTextures;

public:
	cDescription(std::string Title, D3DCOLOR color, int W, int H = FALSE, int PaddingTop_ = FALSE, int PaddingBottom_ = FALSE, std::shared_ptr<cProgressBar> ProgressBar = nullptr);
	void SetIcon(LPDIRECT3DTEXTURE9 lpTexture_, RECT* rct);
	void SetBodyStringShort(std::shared_ptr<cMsgWithSeparator> pstrBody);
	void PushRequerimentInfo(std::shared_ptr<cMsgWithSeparator> pstrBody, std::vector<int> vRequeriments, int* pTriggerRequeriment, int* pTriggerPosition);
	void PushMiltiInfo(std::shared_ptr<cMsgWithSeparator> pstrBody, std::vector<int> vValues_, D3DCOLOR ColorOff, D3DCOLOR ColorOn, int* ptrTrigger = nullptr, BOOL bUseTrigger = FALSE, BOOL bUseTriggerWithPos = FALSE, BOOL bUsePercent = FALSE);
	void PushMiltiInfoString(std::shared_ptr<cMsgWithSeparator> pstrBody, std::string sValue, D3DCOLOR ColorOff, D3DCOLOR ColorOn, int* ptrTrigger = nullptr, BOOL bUseTrigger = FALSE, BOOL bUseTriggerWithBool = FALSE);
	void SetBodyString(std::shared_ptr<cMsgWithSeparator> pstrBody, int* pTriggerCurrent = nullptr, int* pTriggerCnt = nullptr, int* pFinish = nullptr);
	void SetSeparatorWidth(int Width);
	void SetTitle(std::string Title) { strTitle = Title; };
	void Draw(int x, int y);

};