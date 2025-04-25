#pragma once
#include "Base.h"
#include "ProtosDef.h"

static const unsigned int RECV_XORKEYNUM = 16384;
static const unsigned int RECV_XORSEED = (0x8F54C37B | 1);
static const unsigned int SEND_XORKEY	= 0x7AB38CF1;

enum eGameSocket
{
	Login = 0x1163498,
	World = 0x116349C,
	Party = 0x11634A0,
	Chat  = 0x11634A4
};

struct cXorKeyTable
{
	DWORD TablePos;
	DWORD TableGetPos;
	DWORD IsLoaded;
	DWORD* XorKeyTable;
};

namespace PacketManager
{
	void Send(eGameSocket Socket, void* Packet, int Size);
	void __fastcall SetPacketRegister(void* This);
	sS2C_HEADER* __fastcall RecvPacket(int* This);
	namespace Rand
	{
		static const UINT32 dwMask[] = { 0xFFFFFFFF, 0xFFFFFF00, 0xFFFF0000, 0xFF000000 };

		class CKeyRand
		{
		private:
			DWORD _holdrand;

		public:
			void Seed(DWORD dwSeed) { _holdrand = dwSeed; }
			int Rand();
		};

		bool __fastcall CXorKeyTable(cXorKeyTable* XorKeyTable);
		void __fastcall Generate2ndXorKeyTable(cXorKeyTable* XorKeyTable, void* n, int Seed);
		bool __fastcall DecodeHeader(cXorKeyTable* XorKeyTable, void* n, int* packetHeader, bool *bHeaderExtende, DWORD* cpPacket);
		int __fastcall DecodePacket(cXorKeyTable* XorKeyTable, void* n, void* cpPacket, unsigned int iLen);
	}

	void SetHooks();
}