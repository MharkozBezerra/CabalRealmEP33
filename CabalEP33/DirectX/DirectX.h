#pragma once
#include <map>
#include "Base.h"
#include "DirectXTypes.h"
#include "WindowObj/WindowObj.h"

#define Direct3DDevice		(*(LPDIRECT3DDEVICE9*)0x00CEB7F4)
#define DirectGlobalSprite	(*(LPD3DXSPRITE*)0x00CEB844)

namespace DirectX
{
	class DirectX9
	{
	public:
		DirectX9();
		~DirectX9() {};
		LPD3DXFONT			GetLoadedFont(Fonts::D3D9_FontsTypes FindFont);
		void				OnLostAllFonts();
		void				OnResetAllFonts();
		void				ResetFont(LPD3DXFONT pFont);
		void				ResetFont(Fonts::D3D9_FontsTypes FontType);

		LPDIRECT3DTEXTURE9	LoadTexture(std::string Filename, RECT* pRect = nullptr);
		std::shared_ptr<Texture::sTextureWithRect> LoadTextureWithRectByMemory(const char* pBuffer, size_t size);
		std::shared_ptr<Texture::sTextureWithRect> LoadTextureWithRect(std::string Filename);

		LPDIRECT3DTEXTURE9 LoadTextureFromHBitmap(HBITMAP HBitmap, UINT Width, UINT Height, LPDIRECT3DTEXTURE9 lpTexture = nullptr);

		LPDIRECT3DTEXTURE9	FindTextureName(std::string Name);

		int					GetTextWidth(LPD3DXFONT pFont, const char* szText);
		void GetTextSize(ID3DXFont* pFont, const char* szText, SIZE* size);
		int					GetTextWidth(Fonts::D3D9_FontsTypes FontType, const char* szText);
		int					GetTextHeight(Fonts::D3D9_FontsTypes FontType, RECT* pRect, const char* szText);

		void				DrawMessage(Fonts::D3D9_FontsTypes FontType, RECT* pRect, D3DCOLOR D3DColor, const char* Message, LPD3DXSPRITE pSprite = nullptr);
		void				DrawMessage(Fonts::D3D9_FontsTypes FontType, RECT* pRect, DWORD Format, D3DCOLOR D3DColor, const char* Message, LPD3DXSPRITE pSprite);
		void				DrawMessage(LPD3DXFONT pFont, RECT* pRect, D3DCOLOR D3DColor, const char* Message, LPD3DXSPRITE pSprite = nullptr);

		void AdjustFontSize();

		void				DrawSprite(LPD3DXSPRITE Sprite, LPDIRECT3DTEXTURE9 Texture, DWORD Flags, float x, float y);
		void				DrawTextureSprite(DirectX::Texture::sTextureWithRect* Texture, DWORD Flags, float x, float y, BYTE Opc = 255);
		void				DrawSprite(LPDIRECT3DTEXTURE9 Texture, DWORD Flags, float x, float y, BYTE Opc = 255);		

		void DrawTextureSprite(LPDIRECT3DTEXTURE9 pTexture, float posX, float posY, float width, float height);

		static LPD3DXSPRITE				GetSprite() { return &*(LPD3DXSPRITE)DirectGlobalSprite; }
		static LPDIRECT3DDEVICE9        GetDevice() { return Direct3DDevice; }

	private:
		BOOL MakeFont(Fonts::D3D9_FontsTypes FontType, INT Height, UINT Width, UINT Weight, UINT MipLevels, BOOL Italic, DWORD  CharSet, DWORD  OutputPrecision, DWORD  Quality, DWORD  PitchAndFamily, LPCTSTR pFacename);
		void MakeAllFonts();

		void LoadAllTextures();
		std::map<uint32_t, std::vector<std::tuple<ID3DXFont*, const char*, SIZE>>> mFontBuffer;
		std::map<Fonts::D3D9_FontsTypes, std::pair<LPD3DXFONT, int>> mLoadFont;
		std::map<std::string, LPDIRECT3DTEXTURE9> mLoadTextures;
	};

	void InitDirectX();
	void InitHooks();

	extern std::shared_ptr<DirectX9> pDirectX;
	extern std::vector<std::shared_ptr<sWindowObject>> pWindowObjects;
}


