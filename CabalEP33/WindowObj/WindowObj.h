#pragma once
#include "Base.h"
#include "DirectX/DirectXTypes.h"

struct sDefPos
{
	int X;
	int Y;
	int sX;
	int sY;
	int W;
	int H;
};

class sWindowObject
{
public:
	BOOL	Flag;
	BOOL	Scale;
	BOOL	StaticObj;
	BOOL	DinamicSave;
	BOOL    isVisible;
	BOOL    bCloseWKey;

	sDefPos ObjPos;
	std::vector<LPDIRECT3DTEXTURE9> Textures;
	std::vector<std::shared_ptr<DirectX::Texture::sTextureWithRect>> TexturesRct;
	virtual void Draw() = 0;
	virtual void Save() = 0;
	virtual BOOL CloseWithKey() = 0;
	virtual BOOL ObjectClick(int x, int y) = 0;
	virtual BOOL OnHover(int x, int y) = 0;
	virtual BOOL ScreenVisible() = 0;
	virtual void ResetSlot() {}
};

