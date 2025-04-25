#pragma once
#include "Base.h"
#include "PacketManager/ProtosDef.h"

#define USE_CUSTOM_VERSION
#define USE_CUSTOM_MAGICKEY
#define USE_CUSTOM_XORKEY

namespace GameVersion
{
	static const BYTE vXorKeys[] = { 0x92, 0x65, 0x67, 0x57 };
	static const unsigned int GAMEVERSION = 263;
	static const unsigned int GAMEMAGICKEY = 583120283;

	void SendGameVersion();
	int __fastcall RecvGameVersion(void* This);
	int __fastcall GetMagicKey(void* This);
	DWORD DecryptXOR(DWORD* pdw);

	void SetHooks();
}