#pragma once
#include "Base.h"
#include "Memory/Memory.h"

static HWND* CabalhWnd = (HWND*)0xCDA3B0;

enum eCabalCursorType
{
	Normal,
	Block,
	Cursor_Attack,
	Dialog,
	Grab,
	Unk,
	Info,
	BikeKit,
	Destroy,
	Extend,
	Optimized,
	SealPetKit,
	RegressionPetKit,
	RenamePetKit,
	Unk1,
	SecurityPetKit,
	Unk2,
	SkinSupport,
	ClassAttCard,
	UntrainPetKit
};


namespace WindowProc
{
	static WNDPROC gameWndProc = (WNDPROC)0x655B17;

	void InitHooks();
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}