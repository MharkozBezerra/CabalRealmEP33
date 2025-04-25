#include "Base.h"
#include "DirectX.h"
#include "GameDef.h"
#include "BM3Sinergy/BM3Sinergy.h"
#include "EventAlert/EventAlert.h"

DirectX::DirectX9::DirectX9()
{	
	LoadAllTextures();
	MakeAllFonts();

	pBM3Sinergy = std::make_shared<BM3Sinergy>(177, 117, 35, 7);
	pWindowObjects.push_back(pBM3Sinergy);

	pEventAlert = std::make_shared<EventAlert>(0, 0, 896, 293);
	pWindowObjects.push_back(pEventAlert);
}

void DirectX::DirectX9::LoadAllTextures()
{
	//mLoadTextures.insert(std::pair<std::string, LPDIRECT3DTEXTURE9>("PA&PD_BG", LoadTexture("Data\\SyncGames\\PA&PD\\Bg.png")));
}

LPDIRECT3DTEXTURE9 DirectX::DirectX9::FindTextureName(std::string Name)
{
	std::map<std::string, LPDIRECT3DTEXTURE9>::iterator it = mLoadTextures.find(Name);
	if (it != mLoadTextures.end())
		return it->second;
	return nullptr;
}

void DirectX::DirectX9::MakeAllFonts()
{
	MakeFont(Fonts::D3D9_FontsTypes::DIARY_NORMAL, 15, 0, FW_NORMAL, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
	MakeFont(Fonts::D3D9_FontsTypes::DIARY_BOLD, 20, 0, FW_BOLD, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	MakeFont(Fonts::D3D9_FontsTypes::DGDROPS_NORMAL, 15, 0, FW_NORMAL, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
	MakeFont(Fonts::D3D9_FontsTypes::DGDROPS_BOLD, 20, 0, FW_BOLD, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	MakeFont(Fonts::D3D9_FontsTypes::ARIAL_NORMAL, 13, 0, FW_NORMAL, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
	MakeFont(Fonts::D3D9_FontsTypes::ARIAL_BOLD, 13, 0, FW_BOLD, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	MakeFont(Fonts::D3D9_FontsTypes::TAHOMA_NORMAL, 13, 0, FW_NORMAL, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma");
	MakeFont(Fonts::D3D9_FontsTypes::TAHOMA_BOLD, 13, 0, FW_BOLD, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma");

	MakeFont(Fonts::D3D9_FontsTypes::CREATE_SCREEN, 20, 0, FW_NORMAL, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma");
	MakeFont(Fonts::D3D9_FontsTypes::CREATE_SCREEN_TITLE, 24, 0, FW_NORMAL, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma");
	MakeFont(Fonts::D3D9_FontsTypes::CREATE_SCREEN_BASE, 16, 0, FW_NORMAL, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma");
	MakeFont(Fonts::D3D9_FontsTypes::CREATE_SCREEN_WIDGET_TITLE, 28, 0, FW_NORMAL, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma");
	MakeFont(Fonts::D3D9_FontsTypes::CREATE_SCREEN_WIDGET_DESC, 20, 0, FW_NORMAL, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma");

	MakeFont(Fonts::D3D9_FontsTypes::OVERLORD_LEVEL, 28, 0, FW_NORMAL, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Marcellus SC");
	MakeFont(Fonts::D3D9_FontsTypes::OVERLORD_EXP, 14, 0, FW_NORMAL, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Marcellus SC");
	//	InitFontSize();
}

BOOL DirectX::DirectX9::MakeFont(Fonts::D3D9_FontsTypes FontType, INT Height, UINT Width, UINT Weight, UINT MipLevels, BOOL Italic, DWORD  CharSet, DWORD  OutputPrecision, DWORD  Quality, DWORD  PitchAndFamily, LPCTSTR pFacename)
{
		LPD3DXFONT lpFont;
	if (D3DXCreateFontA(GetDevice(), Height, Width, Weight, MipLevels, Italic, CharSet, OutputPrecision, Quality, PitchAndFamily, pFacename, &lpFont) == S_OK)
	{
		mLoadFont.insert(std::pair<Fonts::D3D9_FontsTypes, std::pair<LPD3DXFONT, int>>(FontType, std::pair< LPD3DXFONT, int>(lpFont, Height)));
		return TRUE;
	}

	return FALSE;
}

LPD3DXFONT DirectX::DirectX9::GetLoadedFont(Fonts::D3D9_FontsTypes FindFont)
{
	std::map<Fonts::D3D9_FontsTypes, std::pair<LPD3DXFONT, int>>::iterator it = mLoadFont.find(FindFont);
	if (it != mLoadFont.end())
		return it->second.first;
	return nullptr;
}

LPDIRECT3DTEXTURE9 DirectX::DirectX9::LoadTexture(std::string Filename, RECT* pRect)
{
	LPDIRECT3DTEXTURE9 pTexture = nullptr;
	std::string output = "";

	auto LoadImageTexture = [&](const char* pBuffer, int size) -> LPDIRECT3DTEXTURE9
		{
			D3DXIMAGE_INFO TextureInfo = { 0 };
			D3DXGetImageInfoFromFileInMemory(pBuffer, size, &TextureInfo);


			if (pRect)
			{
				pRect->right = TextureInfo.Width;
				pRect->bottom = TextureInfo.Height;
			}
			if (FAILED(D3DXCreateTextureFromFileInMemoryEx(GetDevice(), pBuffer, size, TextureInfo.Width, TextureInfo.Height, D3DX_FROM_FILE, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_FILTER_POINT, D3DX_DEFAULT, 0xFF000000, &TextureInfo, NULL, &pTexture)))
				return nullptr;

			return pTexture;
		};


	std::ifstream iFile(Filename, std::ifstream::in | std::ifstream::binary);
	if (iFile)
	{
		iFile.seekg(NULL, std::ios::end);
		int size = static_cast<int>(iFile.tellg());
		char* pBuffer = new char[size];
		iFile.seekg(NULL, std::ios::beg);
		iFile.read(pBuffer, size);
		iFile.close();

		if (!LoadImageTexture(pBuffer, size))
		{
			delete[] pBuffer;
			return nullptr;
		}
	}
	else
		return nullptr;

	return pTexture;
	
}

std::shared_ptr<DirectX::Texture::sTextureWithRect> DirectX::DirectX9::LoadTextureWithRectByMemory(const char* pBuffer, size_t size)
{
	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	D3DXIMAGE_INFO TextureInfo = { 0 };
	D3DXGetImageInfoFromFileInMemory(pBuffer, size, &TextureInfo);

	if (FAILED(D3DXCreateTextureFromFileInMemoryEx(GetDevice(), pBuffer, size, TextureInfo.Width, TextureInfo.Height, D3DX_FROM_FILE, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_FILTER_POINT, D3DX_DEFAULT, 0xFF000000, &TextureInfo, NULL, &pTexture)))
		return nullptr;

	if (std::shared_ptr<DirectX::Texture::sTextureWithRect> pTextureRct = std::make_shared<DirectX::Texture::sTextureWithRect>())
	{
		pTextureRct->Rect.right = TextureInfo.Width;
		pTextureRct->Rect.bottom = TextureInfo.Height;
		pTextureRct->lpTexture = pTexture;

		return pTextureRct;
	}

	return nullptr;
}

std::shared_ptr<DirectX::Texture::sTextureWithRect> DirectX::DirectX9::LoadTextureWithRect(std::string Filename)
{
	LPDIRECT3DTEXTURE9 pTexture = nullptr;
	std::string output = "";

	auto LoadImageTexture = [&](const char* pBuffer, int size) -> std::shared_ptr<DirectX::Texture::sTextureWithRect>
		{
			D3DXIMAGE_INFO TextureInfo = { 0 };
			D3DXGetImageInfoFromFileInMemory(pBuffer, size, &TextureInfo);

			if (FAILED(D3DXCreateTextureFromFileInMemoryEx(GetDevice(), pBuffer, size, TextureInfo.Width, TextureInfo.Height, D3DX_FROM_FILE, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_FILTER_POINT, D3DX_DEFAULT, 0xFF000000, &TextureInfo, NULL, &pTexture)))
				return nullptr;

			if (std::shared_ptr<DirectX::Texture::sTextureWithRect> pTextureRct = std::make_shared<DirectX::Texture::sTextureWithRect>())
			{
				pTextureRct->Rect.right = TextureInfo.Width;
				pTextureRct->Rect.bottom = TextureInfo.Height;
				pTextureRct->lpTexture = pTexture;

				return pTextureRct;
			}
			return nullptr;
		};

	std::ifstream iFile(Filename, std::ifstream::in | std::ifstream::binary);
	if (iFile)
	{
		iFile.seekg(NULL, std::ios::end);
		int size = static_cast<int>(iFile.tellg());
		char* pBuffer = new char[size];
		iFile.seekg(NULL, std::ios::beg);
		iFile.read(pBuffer, size);
		iFile.close();

		if (auto pImgTexture = LoadImageTexture(pBuffer, size))
			return pImgTexture;
		else
		{
			delete[] pBuffer;
			return nullptr;
		}

		return nullptr;
	}
	else
		return nullptr;
}


LPDIRECT3DTEXTURE9 DirectX::DirectX9::LoadTextureFromHBitmap(HBITMAP HBitmap, UINT Width, UINT Height, LPDIRECT3DTEXTURE9 lpTexture)
{
	D3DXIMAGE_INFO info;
	ZeroMemory(&info, sizeof(info));
	info.Width = Width;
	info.Height = Height;
	info.Depth = 1;
	info.MipLevels = 1;
	info.Format = D3DFMT_A8R8G8B8;
	info.ResourceType = D3DRTYPE_TEXTURE;

	LPDIRECT3DTEXTURE9 pTexture = lpTexture;

	if (!pTexture)
		DirectX::DirectX9::GetDevice()->CreateTexture(info.Width, info.Height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture, nullptr);
	if (pTexture)
	{
		D3DLOCKED_RECT lockedRect;
		pTexture->LockRect(0, &lockedRect, NULL, D3DLOCK_DISCARD);
		BITMAP bitmap;
		GetObject(HBitmap, sizeof(bitmap), &bitmap);

		memcpy((BYTE*)lockedRect.pBits, (BYTE*)bitmap.bmBits, bitmap.bmWidthBytes * info.Height);

		pTexture->UnlockRect(0);
	}
	return pTexture;
}

int DirectX::DirectX9::GetTextWidth(LPD3DXFONT pFont, const char* szText)
{
	RECT rcRect = {};

	if (pFont)
		pFont->DrawText(NULL, szText, strlen(szText), &rcRect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));

	return rcRect.right - rcRect.left;
}

void DirectX::DirectX9::GetTextSize(ID3DXFont* pFont, const char* szText, SIZE* size)
{
	RECT rcRect = { 0, 0, 0, 0 };

	pFont->DrawText(NULL, szText, -1, &rcRect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));

	size->cx = rcRect.right - rcRect.left;
	size->cy = rcRect.bottom - rcRect.top;
}

int DirectX::DirectX9::GetTextWidth(Fonts::D3D9_FontsTypes FontType, const char* szText)
{
	RECT rcRect = {};
	LPD3DXFONT pCurFont = GetLoadedFont(FontType);

	if (pCurFont)
		pCurFont->DrawText(NULL, szText, strlen(szText), &rcRect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));

	return rcRect.right - rcRect.left;
}

int DirectX::DirectX9::GetTextHeight(Fonts::D3D9_FontsTypes FontType, RECT* pRect, const char* szText)
{
	LPD3DXFONT pCurFont = GetLoadedFont(FontType);

	if (pCurFont)
		return pCurFont->DrawText(NULL, szText, strlen(szText), pRect, DT_CALCRECT | DT_WORDBREAK, D3DCOLOR_XRGB(0, 0, 0));

	return FALSE;
}

void DirectX::DirectX9::DrawMessage(Fonts::D3D9_FontsTypes FontType, RECT* pRect, D3DCOLOR D3DColor, const char* Message, LPD3DXSPRITE pSprite)
{
	if (LPD3DXFONT pCurFont = GetLoadedFont(FontType))
	{
		pRect->right = (int)(MaxResolutionX / ObjectScaleX);
		pRect->bottom = (int)(MaxResolutionY / ObjectScaleY);
		pCurFont->DrawTextA(pSprite, Message, -1, pRect, 0, D3DColor);
	}
}

void DirectX::DirectX9::DrawMessage(Fonts::D3D9_FontsTypes FontType, RECT* pRect, DWORD Format, D3DCOLOR D3DColor, const char* Message, LPD3DXSPRITE pSprite)
{
	if (LPD3DXFONT pCurFont = GetLoadedFont(FontType))
		pCurFont->DrawTextA(pSprite, Message, -1, pRect, Format, D3DColor);
}

void DirectX::DirectX9::DrawMessage(LPD3DXFONT pFont, RECT* pRect, D3DCOLOR D3DColor, const char* Message, LPD3DXSPRITE pSprite)
{
	pRect->right = (int)(MaxResolutionX / ObjectScaleX);
	pRect->bottom = (int)(MaxResolutionY / ObjectScaleY);
	pFont->DrawTextA(pSprite, Message, -1, pRect, 0, D3DColor);
}

void DirectX::DirectX9::OnLostAllFonts()
{
	for (auto it : mLoadFont)
		if (it.second.first)
			it.second.first->OnLostDevice();
}

void DirectX::DirectX9::OnResetAllFonts()
{
	for (auto it : mLoadFont)
		if (it.second.first)
			it.second.first->OnResetDevice();
}

void DirectX::DirectX9::ResetFont(LPD3DXFONT pFont)
{
	if (pFont)
	{
		pFont->OnLostDevice();
		pFont->OnResetDevice();
	}
}
void DirectX::DirectX9::ResetFont(Fonts::D3D9_FontsTypes FontType)
{
	LPD3DXFONT pCurFont = GetLoadedFont(FontType);
	if (pCurFont)
	{
		pCurFont->OnLostDevice();
		pCurFont->OnResetDevice();
	}
}

void DirectX::DirectX9::DrawTextureSprite(LPDIRECT3DTEXTURE9 pTexture, float posX, float posY, float width, float height)
{
	auto pDevice = GetDevice();

	LPDIRECT3DSTATEBLOCK9 pStateBlock = NULL;
	pDevice->CreateStateBlock(D3DSBT_ALL, &pStateBlock);
	pStateBlock->Capture();

	pDevice->SetVertexShader(NULL);
	pDevice->SetPixelShader(NULL);

	// Resetar texturas
	for (DWORD i = 0; i < 8; ++i) {
		pDevice->SetTexture(i, NULL);
	}

	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer = NULL;
	if (FAILED(pDevice->CreateVertexBuffer(4 * sizeof(DirectX::Vertex::CUSTOMVERTEX), 0, (D3DFVF_XYZRHW | D3DFVF_TEX1), D3DPOOL_DEFAULT, &pVertexBuffer, NULL))) {
		pStateBlock->Apply();
		pStateBlock->Release();
		return;
	}

	DirectX::Vertex::CUSTOMVERTEX vertices[] = {
		{ posX, posY, 0.5f, 1.0f, 0.0f, 0.0f },
		{ posX + width, posY, 0.5f, 1.0f, 1.0f, 0.0f },
		{ posX, posY + height, 0.5f, 1.0f, 0.0f, 1.0f },
		{ posX + width, posY + height, 0.5f, 1.0f, 1.0f, 1.0f }
	};

	void* pVertices;
	pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	memcpy(pVertices, vertices, sizeof(vertices));
	pVertexBuffer->Unlock();

	pDevice->SetTexture(0, pTexture);
	pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(DirectX::Vertex::CUSTOMVERTEX));
	pDevice->SetFVF((D3DFVF_XYZRHW | D3DFVF_TEX1));
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pVertexBuffer->Release();

	pStateBlock->Apply();
	pStateBlock->Release();
}

void DirectX::DirectX9::DrawTextureSprite(DirectX::Texture::sTextureWithRect* Texture, DWORD Flags, float x, float y, BYTE Opc)
{
	DrawTextureSprite(Texture->lpTexture, x, y, Texture->Rect.right, Texture->Rect.bottom);
}

void DirectX::DirectX9::DrawSprite(LPDIRECT3DTEXTURE9 Texture, DWORD Flags, float x, float y, BYTE Opc)
{
	D3DXVECTOR3 ImgPos(x, y, 0);
	LPD3DXSPRITE pSprite = GetSprite();
	D3DXMATRIX oldMatrix, newMatrix;

	pSprite->Begin(Flags);
	pSprite->GetTransform(&oldMatrix);
	pSprite->SetTransform(D3DXMatrixScaling(&newMatrix, ObjectScaleX, ObjectScaleY, 0));
	pSprite->Draw(Texture, nullptr, nullptr, &ImgPos, D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, Opc));
	pSprite->SetTransform(&oldMatrix);
	pSprite->End();
}

void DirectX::DirectX9::DrawSprite(LPD3DXSPRITE Sprite, LPDIRECT3DTEXTURE9 Texture, DWORD Flags, float x, float y)
{
	D3DXVECTOR3 ImgPos(x, y, 0);

	if (Sprite)
	{
		Sprite->Begin(Flags);
		Sprite->Draw(Texture, nullptr, nullptr, &ImgPos, -1);
		Sprite->End();
	}
}

void DirectX::InitHooks()
{
	
}

void DirectX::InitDirectX()
{
	pDirectX = std::make_shared<DirectX9>();
}

std::shared_ptr<DirectX::DirectX9> DirectX::pDirectX = nullptr;
std::vector<std::shared_ptr<sWindowObject>> DirectX::pWindowObjects;