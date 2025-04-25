#pragma once
#include <d3d9.h>
#include <d3dx9.h>

namespace DirectX
{
	namespace Texture
	{
		struct sTextureWithRect
		{
			LPDIRECT3DTEXTURE9 lpTexture;
			RECT Rect;
		};
	};

	namespace Vertex
	{
		struct CUSTOMVERTEX
		{
			float x, y, z;
			D3DCOLOR color;
			float       tu, tv;
		};

		struct D3DTLVERTEX
		{
			float sx;
			float sy;
			float sz;
			float rhw;
			D3DCOLOR color;
			D3DCOLOR specular;
			float tu;
			float tv;
		};

	};
	namespace Fonts
	{
		enum D3D9_FontsTypes :uint32_t
		{
			ARIAL_NORMAL,
			ARIAL_BOLD,
			TAHOMA_NORMAL,
			TAHOMA_BOLD,
			DIARY_NORMAL,
			DIARY_BOLD,
			DGDROPS_NORMAL,
			DGDROPS_BOLD,
			CREATE_SCREEN,
			CREATE_SCREEN_TITLE,
			CREATE_SCREEN_BASE,
			CREATE_SCREEN_WIDGET_TITLE,
			CREATE_SCREEN_WIDGET_DESC,
			OVERLORD_LEVEL,
			OVERLORD_EXP,
			LAST_FONT
		};

	};
};