#include "Base.h"
#include "cDescription.h"
#include "GameDef.h"
#include "Basic/Basic.h"
#include <sstream>
#include <format>

using namespace Basic;
cDescription::cDescription(std::string Title, D3DCOLOR color, int W, int H, int PaddingTop_, int PaddingBottom_, std::shared_ptr<cProgressBar> ProgressBar)
{
	ZeroMemory(&rct, sizeof(RECT));
	strTitle = Title;
	colorTitle = color;
	rct.right = W;
	rct.bottom = H;
	PaddingTop = PaddingTop_;
	PaddingBottom = PaddingBottom_;
	pLastStr = nullptr;
	pTriggerCur = nullptr;
	pTriggerCnt = nullptr;
	pFinish = nullptr;
	pMsgProgress = nullptr;
	pProgressBar = ProgressBar;

	lpTexture[0] = pDirectX->LoadTexture(std::format("{}\\Data\\UserData\\DescriptionBox\\TopLeft.png", Game::GetGameDir()));
	lpTexture[1] = pDirectX->LoadTexture(std::format("{}\\Data\\UserData\\DescriptionBox\\TopMid.png", Game::GetGameDir()));
	lpTexture[2] = pDirectX->LoadTexture(std::format("{}\\Data\\UserData\\DescriptionBox\\TopRight.png", Game::GetGameDir()));
	lpTexture[3] = pDirectX->LoadTexture(std::format("{}\\Data\\UserData\\DescriptionBox\\MidLeft.png", Game::GetGameDir()));
	lpTexture[4] = pDirectX->LoadTexture(std::format("{}\\Data\\UserData\\DescriptionBox\\MidMid.png", Game::GetGameDir()));
	lpTexture[5] = pDirectX->LoadTexture(std::format("{}\\Data\\UserData\\DescriptionBox\\MidRight.png", Game::GetGameDir()));
	lpTexture[6] = pDirectX->LoadTexture(std::format("{}\\Data\\UserData\\DescriptionBox\\BotLeft.png", Game::GetGameDir()));
	lpTexture[7] = pDirectX->LoadTexture(std::format("{}\\Data\\UserData\\DescriptionBox\\BotMid.png", Game::GetGameDir()));
	lpTexture[8] = pDirectX->LoadTexture(std::format("{}\\Data\\UserData\\DescriptionBox\\BotRight.png", Game::GetGameDir()));
	lpTexture[9] = pDirectX->LoadTexture(std::format("{}\\Data\\UserData\\DescriptionBox\\Separator.png", Game::GetGameDir()));


	tX = ((rct.right - pDirectX->GetTextWidth(Fonts::D3D9_FontsTypes::DIARY_NORMAL, strTitle.c_str())) / 2);
	tY = pDirectX->GetTextHeight(Fonts::D3D9_FontsTypes::DIARY_NORMAL, NULL, strTitle.c_str());
}

void cDescription::SetIcon(LPDIRECT3DTEXTURE9 lpTexture_, RECT* rct)
{
	lpTexture[10] = lpTexture_;
	memcpy(&rctIcon, rct, sizeof(RECT));
}

void cDescription::SetBodyStringShort(std::shared_ptr<cMsgWithSeparator> pstrBody)
{
	vBodyMsg.push_back(pstrBody);
	RECT rct_ = { 10, 0, rct.right - 10, 0 };

	uint32_t iSeparatorDist = (pstrBody->bUseSeparator) ? 6 : 0;
	pstrBody->rctMsg.top = (pLastStr) ? pLastStr->rctMsg.bottom + pstrBody->PaddingTop : 0 + pstrBody->PaddingTop;
	pstrBody->rctMsg.bottom = pstrBody->rctMsg.top + pDirectX->GetTextHeight(Fonts::D3D9_FontsTypes::DIARY_NORMAL, &rct_, pstrBody->strBody.c_str()) + 3 + iSeparatorDist + pstrBody->PaddingBottom;
	pstrBody->rctSeparator.top = pstrBody->rctMsg.bottom - (iSeparatorDist / 2);

	pLastStr = pstrBody;
}

void cDescription::PushRequerimentInfo(std::shared_ptr<cMsgWithSeparator> pstrBody, std::vector<int> vRequeriments, int* pTriggerRequeriment, int* pTriggerPosition)
{
	auto itFind = std::find_if(vMultiInfosMessage.begin(), vMultiInfosMessage.end(), [pstrBody](std::shared_ptr<sDescriptionMultiInfo32> pInfo) {return pstrBody == pInfo->pMessage; });
	if (itFind != vMultiInfosMessage.end())
	{
		if (auto pDescriptionRequeriments = std::make_shared<sDescriptionRequerimentInfo32>())
		{
			pDescriptionRequeriments->vRequeriments = vRequeriments;
			pDescriptionRequeriments->pTriggerRequeriment = pTriggerRequeriment;
			pDescriptionRequeriments->pTriggerPosition = pTriggerPosition;

			(*itFind)->pRequeriments = pDescriptionRequeriments;
		}
	}
}

void cDescription::PushMiltiInfo(std::shared_ptr<cMsgWithSeparator> pstrBody, std::vector<int> vValues_, D3DCOLOR ColorOff, D3DCOLOR ColorOn, int* ptrTrigger, BOOL bUseTrigger, BOOL bUseTriggerWithPos, BOOL bUsePercent)
{
	vBodyMsg.push_back(pstrBody);
	RECT rct_ = { 10, 0, rct.right - 10, 0 };

	uint32_t iSeparatorDist = (pstrBody->bUseSeparator) ? 6 : 0;
	pstrBody->rctMsg.top = (pLastStr) ? pLastStr->rctMsg.bottom + pstrBody->PaddingTop : 0 + pstrBody->PaddingTop;
	pstrBody->rctMsg.bottom = pstrBody->rctMsg.top + pDirectX->GetTextHeight(Fonts::D3D9_FontsTypes::DIARY_NORMAL, &rct_, pstrBody->strBody.c_str()) + 3 + iSeparatorDist + pstrBody->PaddingBottom;
	pstrBody->rctSeparator.top = pstrBody->rctMsg.bottom - (iSeparatorDist / 2);

	pLastStr = pstrBody;

	if (auto pDescriptionMultiInfo = std::make_shared< sDescriptionMultiInfo32>())
	{
		pDescriptionMultiInfo->pMessage = pstrBody;
		pDescriptionMultiInfo->vValues = vValues_;
		pDescriptionMultiInfo->ColorValueOn = ColorOn;
		pDescriptionMultiInfo->ColorValueOff = ColorOff;
		pDescriptionMultiInfo->pTrigger = ptrTrigger;
		pDescriptionMultiInfo->bUseTrigger = bUseTrigger;
		pDescriptionMultiInfo->bUseTriggerWithPos = bUseTriggerWithPos;
		pDescriptionMultiInfo->bUsePercent = bUsePercent;

		vMultiInfosMessage.push_back(pDescriptionMultiInfo);
	}
}

void cDescription::PushMiltiInfoString(std::shared_ptr<cMsgWithSeparator> pstrBody, std::string sValue, D3DCOLOR ColorOff, D3DCOLOR ColorOn, int* ptrTrigger, BOOL bUseTrigger, BOOL bUseTriggerWithBool)
{
	vBodyMsg.push_back(pstrBody);
	RECT rct_ = { 10, 0, rct.right - 10, 0 };

	uint32_t iSeparatorDist = (pstrBody->bUseSeparator) ? 6 : 0;
	pstrBody->rctMsg.top = (pLastStr) ? pLastStr->rctMsg.bottom + pstrBody->PaddingTop : 0 + pstrBody->PaddingTop;
	pstrBody->rctMsg.bottom = pstrBody->rctMsg.top + pDirectX->GetTextHeight(Fonts::D3D9_FontsTypes::DIARY_NORMAL, &rct_, pstrBody->strBody.c_str()) + 3 + iSeparatorDist + pstrBody->PaddingBottom;
	pstrBody->rctSeparator.top = pstrBody->rctMsg.bottom - (iSeparatorDist / 2);

	pLastStr = pstrBody;

	if (auto pDescriptionMultiInfoString = std::make_shared< sDescriptionMultiInfoString>())
	{
		pDescriptionMultiInfoString->pMessage = pstrBody;
		pDescriptionMultiInfoString->sValue = sValue;
		pDescriptionMultiInfoString->ColorValueOn = ColorOn;
		pDescriptionMultiInfoString->ColorValueOff = ColorOff;
		pDescriptionMultiInfoString->pTrigger = ptrTrigger;
		pDescriptionMultiInfoString->bUseTrigger = bUseTrigger;
		pDescriptionMultiInfoString->bUseTriggerWithBool = bUseTriggerWithBool;

		vMultiInfosString.push_back(pDescriptionMultiInfoString);
	}
}

void cDescription::SetBodyString(std::shared_ptr<cMsgWithSeparator> pstrBody, int* pTriggerCurrent, int* pTriggerCount, int* Finish)
{
	vBodyMsg.push_back(pstrBody);
	RECT rct_ = { 10, 0, rct.right - 10, 0 };

	uint32_t iSeparatorDist = (pstrBody->bUseSeparator) ? 6 : 0;
	pstrBody->rctMsg.top = (pLastStr) ? pLastStr->rctMsg.bottom + pstrBody->PaddingTop : 0 + pstrBody->PaddingTop;
	pstrBody->rctMsg.bottom = pstrBody->rctMsg.top + pDirectX->GetTextHeight(Fonts::D3D9_FontsTypes::DIARY_NORMAL, &rct_, pstrBody->strBody.c_str()) + 3 + iSeparatorDist + pstrBody->PaddingBottom;
	pstrBody->rctSeparator.top = pstrBody->rctMsg.bottom - (iSeparatorDist / 2);

	pLastStr = pstrBody;

	if ((pTriggerCur = pTriggerCurrent) && (pTriggerCnt = pTriggerCount) && (pFinish = Finish))
	{
		pMsgProgress = pstrBody;
		if (pProgressBar)
		{
			pstrBody->rctMsg.top += 5;
			pstrBody->rctMsg.bottom += 5;
		}
	}
}

void cDescription::SetSeparatorWidth(int Width)
{
	SeparatorWidth = Width;
}

void cDescription::Draw(int x, int y)
{
	if (!vBodyMsg.size())
		return;

	D3DXMATRIX oldMatrix, newMatrix;

	RECT rctstr = { x + tX, y + 10 + PaddingTop, x + (rct.right - 10),  y + 10 + tY + PaddingBottom };

	RECT rctTopLeft = { 0, 0, 2, 2 };
	RECT rctTopMid = { 2, 0, rct.right - 2, 2 };
	RECT rctTopRight = { rct.right - 2, 0, rct.right, 2 };


	RECT rctMidLeft = { 0, 0, 2, rct.bottom - 2 };
	RECT rctMidMid = { 2, 0, rct.right - 2, rct.bottom - 2 };
	RECT rctMidRight = { rct.right - 2, 0, rct.right, rct.bottom - 2 };
	RECT rctSeparator = { 0, 0, rct.right - 6, 1 };

	if (!rct.bottom)
		rct.bottom = (rctstr.bottom - y) + 8 + vBodyMsg.back()->rctMsg.bottom + 5;

	LPD3DXSPRITE pSprite = pDirectX->GetSprite();
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	pSprite->GetTransform(&oldMatrix);
	pSprite->SetTransform(D3DXMatrixScaling(&newMatrix, ObjectScaleX, ObjectScaleY, 0));

	pSprite->Draw(lpTexture[0], &rctTopLeft, nullptr, &D3DXVECTOR3(static_cast<float>(x), static_cast<float>(y), 0), D3DCOLOR_XRGB(255, 255, 255));
	pSprite->Draw(lpTexture[1], &rctTopMid, nullptr, &D3DXVECTOR3(static_cast<float>(x + 2), static_cast<float>(y), 0), D3DCOLOR_XRGB(255, 255, 255));
	pSprite->Draw(lpTexture[2], &rctTopRight, nullptr, &D3DXVECTOR3(static_cast<float>(x + (rct.right - 2)), static_cast<float>(y), 0), D3DCOLOR_XRGB(255, 255, 255));


	pSprite->Draw(lpTexture[3], &rctMidLeft, nullptr, &D3DXVECTOR3(static_cast<float>(x), static_cast<float>(y + 2), 0), D3DCOLOR_XRGB(255, 255, 255));
	pSprite->Draw(lpTexture[4], &rctMidMid, nullptr, &D3DXVECTOR3(static_cast<float>(x + 2), static_cast<float>(y + 2), 0), D3DCOLOR_XRGB(255, 255, 255));
	pSprite->Draw(lpTexture[5], &rctMidRight, nullptr, &D3DXVECTOR3(static_cast<float>(x + (rct.right - 2)), static_cast<float>(y + 2), 0), D3DCOLOR_XRGB(255, 255, 255));


	pSprite->Draw(lpTexture[6], &rctTopLeft, nullptr, &D3DXVECTOR3(static_cast<float>(x), static_cast<float>(y + rct.bottom), 0), D3DCOLOR_XRGB(255, 255, 255));
	pSprite->Draw(lpTexture[7], &rctTopMid, nullptr, &D3DXVECTOR3(static_cast<float>(x + 2), static_cast<float>(y + rct.bottom), 0), D3DCOLOR_XRGB(255, 255, 255));
	pSprite->Draw(lpTexture[8], &rctTopRight, nullptr, &D3DXVECTOR3(static_cast<float>(x + (rct.right - 2)), static_cast<float>(y + rct.bottom), 0), D3DCOLOR_XRGB(255, 255, 255));

	pDirectX->DrawMessage(Fonts::D3D9_FontsTypes::DIARY_NORMAL, &rctstr, DT_NOCLIP | DT_LEFT | DT_WORDBREAK, colorTitle, strTitle.c_str(), pSprite);

	for (std::vector<std::shared_ptr<cMsgWithSeparator> >::iterator it = vBodyMsg.begin(); it != vBodyMsg.end(); it++)
	{
		if ((pMsgProgress == *it) && pProgressBar && pProgressBar->lpTextureBar && pProgressBar->lpTextureBar_)
		{
			if (LPD3DXFONT lpFont = pDirectX->GetLoadedFont(Fonts::D3D9_FontsTypes::DIARY_NORMAL))
			{
				D3DXMATRIX mat, matScale;
				std::string strProgress = Useful::ConcatValue(*pTriggerCur) + "/" + Useful::ConcatValue(*pTriggerCnt);
				RECT rctSprite = { 0, 0, static_cast<int>(((*pTriggerCur) * pProgressBar->lpTextureBar_->Rect.right) / *pTriggerCnt), pProgressBar->lpTextureBar_->Rect.bottom };

				RECT rctFont = { x + ((rct.right - 219) / 2) + ((pProgressBar->lpTextureBar->Rect.right - pDirectX->GetTextWidth(Fonts::D3D9_FontsTypes::DIARY_NORMAL, strProgress.c_str())) / 2), (rctstr.bottom + 8) + (*it)->rctMsg.top + ((pProgressBar->lpTextureBar->Rect.bottom - pDirectX->GetTextHeight(Fonts::D3D9_FontsTypes::DIARY_NORMAL, nullptr, strProgress.c_str())) / 2), x + (rct.right - 10), (rctstr.bottom + 8) + (*it)->rctMsg.top + pProgressBar->lpTextureBar->Rect.bottom };

				pSprite->Draw(pProgressBar->lpTextureBar->lpTexture, nullptr, nullptr, &D3DXVECTOR3(static_cast<float>(x + ((rct.right - 219) / 2)), static_cast<float>((rctstr.bottom + 8) + (*it)->rctMsg.top), 0), D3DCOLOR_XRGB(255, 255, 255));
				pSprite->Draw(pProgressBar->lpTextureBar_->lpTexture, &rctSprite, nullptr, &D3DXVECTOR3(static_cast<float>(x + 1 + ((rct.right - 219) / 2)), static_cast<float>((rctstr.bottom + 8) + (*it)->rctMsg.top + 1), 0), D3DCOLOR_XRGB(255, 255, 255));

				lpFont->DrawTextA(pSprite, strProgress.c_str(), strProgress.length(), &rctFont, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

			}
		}
		else
		{
			std::string strFormat = (pMsgProgress == *it) ? Useful::FindAndReplace((*it)->strBody, "%d/%d", Useful::ConcatValue(*pTriggerCur) + "/" + Useful::ConcatValue(*pTriggerCnt)) : (*it)->strBody;
			RECT rctbody = { x + 10, (rctstr.bottom + 8) + (*it)->rctMsg.top, x + (rct.right - 10), rct.bottom };
			if ((*it)->bCenter)
			{
				if (LPD3DXFONT lpFont = pDirectX->GetLoadedFont(Fonts::D3D9_FontsTypes::DIARY_NORMAL))
				{
					SIZE szStr = {};
					pDirectX->GetTextSize(lpFont, strFormat.c_str(), &szStr);
					RECT rctbody2 = { x + ((rct.right - szStr.cx) / 2), (rctstr.bottom + 8) + (*it)->rctMsg.top, x + (rct.right - 10), rct.bottom };

					pDirectX->DrawMessage(Fonts::D3D9_FontsTypes::DIARY_NORMAL, &rctbody2, DT_NOCLIP | DT_LEFT | DT_WORDBREAK, (*it)->color, strFormat.c_str(), pSprite);
				}
			}
			else
			{
				pDirectX->DrawMessage(Fonts::D3D9_FontsTypes::DIARY_NORMAL, &rctbody, DT_NOCLIP | DT_LEFT | DT_WORDBREAK, (*it)->color, strFormat.c_str(), pSprite);
			}

			auto itFindString = std::find_if(vMultiInfosString.begin(), vMultiInfosString.end(), [it](std::shared_ptr<sDescriptionMultiInfoString> pInfo) {return (*it) == pInfo->pMessage; });
			if (itFindString != vMultiInfosString.end())
			{
				auto pDescriptionMultiInfoString = *itFindString;
				if (LPD3DXFONT lpFont = pDirectX->GetLoadedFont(Fonts::D3D9_FontsTypes::DIARY_NORMAL))
				{
					SIZE szStart{};

					if (pDescriptionMultiInfoString->bUseTriggerWithBool)
					{
						pDirectX->GetTextSize(lpFont, pDescriptionMultiInfoString->sValue.c_str(), &szStart);

						RECT rectRightText = { x + (rct.right - 10) - (szStart.cx), (rctstr.bottom + 8) + (*it)->rctMsg.top, x + (rct.right - 10), rct.bottom };
						pDirectX->DrawMessage(Fonts::D3D9_FontsTypes::DIARY_NORMAL, &rectRightText, DT_NOCLIP | DT_LEFT, (*pDescriptionMultiInfoString->pTrigger) ? pDescriptionMultiInfoString->ColorValueOn : pDescriptionMultiInfoString->ColorValueOff, pDescriptionMultiInfoString->sValue.c_str(), pSprite);
					}
				}
			}
			else
			{
				auto itFind = std::find_if(vMultiInfosMessage.begin(), vMultiInfosMessage.end(), [it](std::shared_ptr<sDescriptionMultiInfo32> pInfo) {return (*it) == pInfo->pMessage; });
				if (itFind != vMultiInfosMessage.end())
				{
					auto pDescriptionMultiInfo = *itFind;
					if (pDescriptionMultiInfo->pTrigger || pDescriptionMultiInfo->pRequeriments)
					{
						if (LPD3DXFONT lpFont = pDirectX->GetLoadedFont(Fonts::D3D9_FontsTypes::DIARY_NORMAL))
						{
							int marginX = 0;
							SIZE szStart{};
							if (auto pRequeriments = pDescriptionMultiInfo->pRequeriments)
							{
								size_t iValue = *pRequeriments->pTriggerPosition;
								if (iValue >= pRequeriments->vRequeriments.size())
									iValue = pRequeriments->vRequeriments.size() - 1;

								if (pRequeriments->vRequeriments.size())
								{
									std::string sValue = std::to_string(pRequeriments->vRequeriments[iValue]);
									pDirectX->GetTextSize(lpFont, sValue.c_str(), &szStart);

									RECT rectRightText = { x + (rct.right - 10) - (szStart.cx), (rctstr.bottom + 8) + (*it)->rctMsg.top, x + (rct.right - 10), rct.bottom };
									pDirectX->DrawMessage(Fonts::D3D9_FontsTypes::DIARY_NORMAL, &rectRightText, DT_NOCLIP | DT_LEFT, (*pRequeriments->pTriggerRequeriment >= pRequeriments->vRequeriments[iValue]) ? pDescriptionMultiInfo->ColorValueOn : pDescriptionMultiInfo->ColorValueOff, sValue.c_str(), pSprite);
								}
							}
							else
							{
								if (pDescriptionMultiInfo->bUseTrigger)
								{
									std::stringstream ss;

									for (size_t szPos = 0; szPos < pDescriptionMultiInfo->vValues.size(); szPos++)
									{
										ss << pDescriptionMultiInfo->vValues[szPos];
										if (pDescriptionMultiInfo->bUsePercent)
											ss << "%";
										if (szPos)
										{
											ss << "/";
											marginX += 8;
										}
									}

									pDirectX->GetTextSize(lpFont, ss.str().c_str(), &szStart);
									szStart.cx += marginX;


									for (size_t i = 0; i < pDescriptionMultiInfo->vValues.size(); i++)
									{
										int value = pDescriptionMultiInfo->vValues[i];

										SIZE szText{};
										SIZE szBar{};
										std::string sValue = std::to_string(value);
										std::string sBar = "/";

										if (pDescriptionMultiInfo->bUsePercent)
											sValue.append("%");

										RECT rectRightText = { x + (rct.right - 10) - (szStart.cx), (rctstr.bottom + 8) + (*it)->rctMsg.top, x + (rct.right - 10), rct.bottom };

										if (i)
										{
											szStart.cx -= 4;
											rectRightText.left = x + (rct.right - 10) - (szStart.cx);
											pDirectX->GetTextSize(lpFont, sBar.c_str(), &szBar);
											pDirectX->DrawMessage(Fonts::D3D9_FontsTypes::DIARY_NORMAL, &rectRightText, DT_NOCLIP | DT_LEFT, pDescriptionMultiInfo->ColorValueOff, sBar.c_str(), pSprite);
											szStart.cx -= szBar.cx + 4;
										}

										rectRightText.left = x + (rct.right - 10) - (szStart.cx);

										pDirectX->DrawMessage(Fonts::D3D9_FontsTypes::DIARY_NORMAL, &rectRightText, DT_NOCLIP | DT_LEFT, (i + 1 == *pDescriptionMultiInfo->pTrigger) ? pDescriptionMultiInfo->ColorValueOn : pDescriptionMultiInfo->ColorValueOff, sValue.c_str(), pSprite);

										pDirectX->GetTextSize(lpFont, sValue.c_str(), &szText);
										szStart.cx -= szText.cx;
									}
								}
								else if (pDescriptionMultiInfo->bUseTriggerWithPos)
								{
									size_t iValue = *pDescriptionMultiInfo->pTrigger;
									if (iValue >= pDescriptionMultiInfo->vValues.size())
										iValue = pDescriptionMultiInfo->vValues.size() - 1;

									if (pDescriptionMultiInfo->vValues.size())
									{
										std::string sValue = std::to_string(pDescriptionMultiInfo->vValues[iValue]);
										if (pDescriptionMultiInfo->bUsePercent)
											sValue.append("%");

										pDirectX->GetTextSize(lpFont, sValue.c_str(), &szStart);

										RECT rectRightText = { x + (rct.right - 10) - (szStart.cx), (rctstr.bottom + 8) + (*it)->rctMsg.top, x + (rct.right - 10), rct.bottom };
										pDirectX->DrawMessage(Fonts::D3D9_FontsTypes::DIARY_NORMAL, &rectRightText, DT_NOCLIP | DT_LEFT, pDescriptionMultiInfo->ColorValueOn, sValue.c_str(), pSprite);
									}
								}
								else
								{
									std::string sValue = std::to_string(*pDescriptionMultiInfo->pTrigger);
									pDirectX->GetTextSize(lpFont, sValue.c_str(), &szStart);

									RECT rectRightText = { x + (rct.right - 10) - (szStart.cx), (rctstr.bottom + 8) + (*it)->rctMsg.top, x + (rct.right - 10), rct.bottom };
									pDirectX->DrawMessage(Fonts::D3D9_FontsTypes::DIARY_NORMAL, &rectRightText, DT_NOCLIP | DT_LEFT, pDescriptionMultiInfo->ColorValueOn, sValue.c_str(), pSprite);
								}
							}
						}
					}
				}
			}
			if ((*it)->bUseSeparator)
			{
				pSprite->Draw(lpTexture[9], &rctSeparator, nullptr, &D3DXVECTOR3((float)(x + ((rct.right - rctSeparator.right) / 2)), (float)((rctstr.bottom + 8) + (*it)->rctSeparator.top), 0), D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF));
			}
		}
	}

	pSprite->Draw(lpTexture[9], &rctSeparator, nullptr, &D3DXVECTOR3((float)(x + ((rct.right - rctSeparator.right) / 2)), (float)rctstr.bottom + 4, 0), D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF));

	pSprite->SetTransform(&oldMatrix);
	pSprite->End();

	if (lpTexture[10])
		pDirectX->DrawSprite(lpTexture[10], D3DXSPRITE_ALPHABLEND, (float)(x + rctIcon.left), (float)(y + rctIcon.top));
}
