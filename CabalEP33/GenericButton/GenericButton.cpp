#include "Base.h"
#include "GenericButton.h"
#include "DirectX/DirectX.h"
#include "Screen/Screen.h"
#include "GameDef.h"

using namespace DirectX;
using namespace Screen;

CGenericButton::CGenericButton(int eType, int eSubType, int x, int y, BOOL bClicked) : PosX(x), PosY(y), TypeButton(eType), SubTypeButton(eSubType), bClicked(bClicked)
{
	bShow = TRUE;
	bHover = FALSE;
}

CGenericButton::CGenericButton(int eType, int eSubType, std::string strDir, std::string _strDir, int x, int y, BOOL bClicked) : TypeButton(eType), SubTypeButton(eSubType), bClicked(bClicked)
{
	bShow = TRUE;
	bHover = FALSE;

	if (!strDir.empty()) vTextures.push_back(pDirectX->LoadTextureWithRect(strDir));
	if (!_strDir.empty()) vTextures.push_back(pDirectX->LoadTextureWithRect(_strDir));

	for (size_t i = 0; i < vTextures.size(); i++)
	{
		if (vTextures.at(i))
		{
			vTextures.at(i)->Rect.left = x;
			vTextures.at(i)->Rect.top = y;
		}
	}
}

void CGenericButton::SetText(std::string _strText, std::string _strSwap, eAlignType align, DirectX::Fonts::D3D9_FontsTypes Font, D3DCOLOR fColor, RECT* rct)
{
	bSwapText = FALSE;
	vStrText.push_back(_strText);
	vStrText.push_back(_strSwap);
	eAlignText = align;
	FontType = Font;
	FontColor = fColor;

	if (rct) memcpy(&rectStr, rct, sizeof(RECT));
}

void CGenericButton::Draw(int x, int y)
{
	if (!bShow)
		return;

	if (fnDraw)
	{
		fnDraw(x, y);
		return;
	}

	if (vTextures.at(0))
	{
		bHover = IsMousePos(pRealCursor.x, pRealCursor.y, (int)(ObjectScaleX * (x + vTextures.at(0)->Rect.left)), (int)(ObjectScaleY * (y + vTextures.at(0)->Rect.top)), (int)(ObjectScaleX * vTextures.at(0)->Rect.right), (int)(ObjectScaleY * vTextures.at(0)->Rect.bottom));
		bool bLocalHover = (bClicked || bHover);
		if (vTextures.at(bLocalHover))
			pDirectX->DrawSprite(vTextures.at(bLocalHover)->lpTexture, D3DXSPRITE_ALPHABLEND, (float)x + vTextures.at(bLocalHover)->Rect.left, (float)y + vTextures.at(bLocalHover)->Rect.top);
	}

	mDrawText(x, y);
}

void CGenericButton::mDrawText(int x, int y)
{
	if (vStrText.size())
	{
		D3DXMATRIX oldMatrix, newMatrix;
		LPD3DXSPRITE pSprite = pDirectX->GetSprite();

		pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		pSprite->GetTransform(&oldMatrix);
		pSprite->SetTransform(D3DXMatrixScaling(&newMatrix, ObjectScaleX, ObjectScaleY, 0));

		RECT rct_msg = { x + vTextures.at(0)->Rect.left + rectStr.left, y + vTextures.at(0)->Rect.top + rectStr.top };
		BOOL bLocalSwap = (!bTriggerSwap) ? bSwapText : *bTriggerSwap;

		if (eAlignText == eAlignType::ALIGN_CENTER)
		{
			RECT rct_calc = {};
			int Width = pDirectX->GetTextWidth(FontType, vStrText[bLocalSwap].c_str());
			int Height = pDirectX->GetTextHeight(FontType, &rct_calc, vStrText[bLocalSwap].c_str());
			rct_msg = { x + vTextures.at(0)->Rect.left + ((vTextures.at(0)->Rect.right - Width) / 2), y + vTextures.at(0)->Rect.top + ((vTextures.at(0)->Rect.bottom - Height) / 2) };
		}

		pDirectX->DrawMessage(FontType, &rct_msg, FontColor, vStrText[bLocalSwap].c_str(), pSprite);

		pSprite->SetTransform(&oldMatrix);
		pSprite->End();
	}
}

void CGenericButton::Click(int x, int y)
{
	if (!bShow)
		return;

	if (bHover && !bClicked)
		Action();
}

void CGenericButton::Action()
{
	if (bLocked)
		return;

	if (vfnAction.size())
		for (auto fnAction : vfnAction)
			fnAction();

	if (uniqueAction)
		uniqueAction();

	uniqueAction = 0;
}