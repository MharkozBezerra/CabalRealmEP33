#pragma once
#include "Base.h"
#include "WindowObj/WindowObj.h"
#include "functional"

enum eAlignType
{
	ALIGN_NONE,
	ALIGN_CENTER,
	ALIGN_FLOAT
};

class CGenericButton
{
public:
	CGenericButton(int eType, int eSubType, int x, int y, BOOL bClicked);

	CGenericButton(int eType, int eSubType, std::string strDir, std::string _strDir, int x, int y, BOOL bClicked = FALSE);

	void SwapText() { bSwapText = !bSwapText; };

	void SetTriggerSwap(BOOL* pSwap) { bTriggerSwap = pSwap; };

	void SetText(std::string strText, std::string strSwap, eAlignType align, DirectX::Fonts::D3D9_FontsTypes Font, D3DCOLOR fColor, RECT* rct = nullptr);

	BOOL IsShow() { return bShow; };

	void Show() { bShow = TRUE; };

	void Hide() { bShow = FALSE; };

	void SetClicked(BOOL Clicked) { bClicked = Clicked; };

	void SetLocked(BOOL Locked) { bLocked = Locked; };

	void AddAction(std::function<void()> fAction) { vfnAction.push_back(fAction); };

	void AddUniqueAction(std::function<void()> fUniqueAction) { uniqueAction = fUniqueAction; };

	void RunAction() { Action(); };

	void SetDraw(std::function<void(int, int)> fDraw) { fnDraw = fDraw; };

	void Draw(int x, int y);
	void mDrawText(int x, int y);

	void Click(int x, int y);

	void SetHover(BOOL Hover) { bHover = Hover; };
	BOOL GetHover() { return bHover; };

	BOOL GetClicked() { return bClicked; };

	void PushTextures(std::shared_ptr<DirectX::Texture::sTextureWithRect> pTexture) { vTextures.push_back(pTexture); };

	std::vector<std::shared_ptr<DirectX::Texture::sTextureWithRect>> &GetTextures() { return vTextures; };

	int GetType() { return TypeButton; };
	int GetSubType() { return SubTypeButton; };
	std::pair<int, int> GetPos() { return std::pair<int, int>(PosX, PosY); };
private:

	void Action();

	std::vector<std::shared_ptr<DirectX::Texture::sTextureWithRect>> vTextures;
	BOOL bShow = FALSE;
	BOOL bHover = FALSE;
	BOOL bClicked = FALSE;
	BOOL bLocked = FALSE;;
	BOOL bSwapText = FALSE;
	BOOL* bTriggerSwap = nullptr;
	eAlignType eAlignText;
	DirectX::Fonts::D3D9_FontsTypes FontType;
	D3DCOLOR FontColor;
	std::vector<std::string> vStrText;

	RECT rectStr;

	int TypeButton;
	int SubTypeButton;
	std::vector<std::function<void()>> vfnAction;
	std::function<void()> uniqueAction = 0;
	std::function<void(int, int)> fnDraw;

	int PosX;
	int PosY;
};