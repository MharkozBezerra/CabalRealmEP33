#include "Base.h"
#include "PacketManager.h"
#include "Memory/Memory.h"
#include "Discord/Discord.h"
#include "GameVersion/GameVersion.h"
#include "DLLSec/DLLSec.h"

using namespace DLLSec;

int __forceinline PacketManager::Rand::CKeyRand::Rand()
{
	return  ((((_holdrand = _holdrand * 0x2F6B6F5 + 0x14698B7) >> 0x10) * 0x27F41C3 + 0xB327BD) >> 0x10);
}

bool __fastcall PacketManager::Rand::CXorKeyTable(cXorKeyTable* XorKeyTable)
{
	VM_STARTM(__FUNCTION__);
	typedef int(*t_Rand)();
	t_Rand _Rand = (t_Rand)0x00B317BE;

	CKeyRand rd = {};

	XorKeyTable->IsLoaded = TRUE;
	if (XorKeyTable->XorKeyTable = new_game<DWORD*>(0x20000))
	{
		rd.Seed(RECV_XORSEED);
		for (DWORD iN = 0; iN < RECV_XORKEYNUM; ++iN)
		{
			UINT16 wLow = static_cast<UINT16>(rd.Rand());
			UINT16 wHigh = static_cast<UINT16>(rd.Rand());
			XorKeyTable->XorKeyTable[iN] = (wLow & 0xFFFF) | ((wHigh & 0xFFFF) << 16);
		}

		XorKeyTable->TableGetPos = (_Rand() & 0x3FFF);
		XorKeyTable->TablePos = XorKeyTable->XorKeyTable[XorKeyTable->TableGetPos];
		return true;
	}

	return false;
	VM_END;
}

void __fastcall PacketManager::Rand::Generate2ndXorKeyTable(cXorKeyTable* XorKeyTable, void* n, int Seed)
{
	VM_STARTM(__FUNCTION__);
	CKeyRand rd = {};

	rd.Seed(Seed);
	for (DWORD iN = RECV_XORKEYNUM; iN < (RECV_XORKEYNUM * 2); ++iN)
	{
		UINT16 wLow = static_cast<UINT16>(rd.Rand());
		UINT16 wHigh = static_cast<UINT16>(rd.Rand());
		XorKeyTable->XorKeyTable[iN] = (wLow & 0xFFFF) | ((wHigh & 0xFFFF) << 16);
	}
	VM_END;
}

bool __fastcall PacketManager::Rand::DecodeHeader(cXorKeyTable* XorKeyTable, void* n, int* packetHeader, bool* bHeaderExtende, DWORD* cpPacket)
{
	VM_STARTM(__FUNCTION__);
	*bHeaderExtende = FALSE;
	int MagicKey = *cpPacket ^ SEND_XORKEY;

	if (static_cast<WORD>(MagicKey) == MAGIC_CODE)
	{
		*packetHeader = HIWORD(MagicKey);
		return true;
	}

	if (static_cast<WORD>(MagicKey) == MAGIC_CODE_EXTENDED)
	{
		*packetHeader = MAKELONG(HIWORD(MagicKey), LOWORD(XorKeyTable->XorKeyTable[(*cpPacket & 0x3FFF)] ^ cpPacket[1]));
		*bHeaderExtende = true;
		return true;
	}

	return false;
	VM_END;
}

int __fastcall PacketManager::Rand::DecodePacket(cXorKeyTable* XorKeyTable, void* n, void* cpPacket, unsigned int iLen)
{
	VM_STARTM(__FUNCTION__);
	UINT32* pHeader = reinterpret_cast<UINT32*>(cpPacket);
	int dwXorKey = SEND_XORKEY;

	if (USERDATACONTEXT* pUserDataCtx = USERDATACONTEXT::GetpUserDataCtx())
	{
		if (pUserDataCtx->GetOnLogged())
		{
			for (int i = 0, j = 0; i < 3; j = i, i++)
			{
				if (!((pSecAuth) && (pSecAuth->SecAuth[i] && pSecAuth->SecKey[i] && ((j) ? pSecAuth->xOR[i] == (((BYTE*)&pSecAuth->SecAuth[j])[0]) : pSecAuth->SecAuth[i] == (pSecAuth->SecKey[i] ^ pSecAuth->xOR[i]))) && ((pSecAuth->SecAuth[i] ^ ((j) ? (((BYTE*)&pSecAuth->SecAuth[j])[0]) : pSecAuth->xOR[i])) == pSecAuth->SecKey[i]) && (pSecAuth->Count[j] == pSecAuth->Count[i])))
				{
					ZeroMemory(cpPacket, iLen);
					iLen = 0;
				}
			}
		}
	}

	if (!XignCode::CheckThreadList())
	{
		ZeroMemory(cpPacket, iLen);
		iLen = 0;

	}

	if (iLen >> 2)
	{
		for (size_t i = 0; i < iLen / 4; ++i)
		{
			UINT32 OldpPayload = (*pHeader & 0x00003FFF);
			*pHeader++ ^= dwXorKey;
			dwXorKey = XorKeyTable->XorKeyTable[OldpPayload];
		}
	}

	int result = dwXorKey & ~dwMask[(iLen & 3)];
	*pHeader ^= result;

	return result;
	VM_END;
}

void PacketManager::Send(eGameSocket Socket, void* Packet, int Size)
{
	typedef void(__thiscall* t_this) (int, void*, int);
	typedef void(__thiscall* TSendGamePacket)(int);

	t_this EncryptPacket = (t_this)0x006692AA;
	TSendGamePacket SendPacket = (TSendGamePacket)0x006692A5;

	EncryptPacket(*reinterpret_cast<DWORD*>(Socket), Packet, Size);
	SendPacket(*reinterpret_cast<DWORD*>(Socket));
}

void __fastcall PacketManager::SetPacketRegister(void* This)
{
	typedef void(__thiscall* TStartUP)(void*);
	TStartUP StartUP = (TStartUP)0x0093BE56;
	VM_STARTU(__FUNCTION__);
	Memory::WriteValue<DWORD>(0x11751C0, reinterpret_cast<DWORD>(Discord::Hook::UpdateMap));
	Memory::WriteValue<DWORD>(0x11720F4, reinterpret_cast<DWORD>(DLLSec::RecvChannelList));

	return StartUP(This);
	VM_END;
}

sS2C_HEADER* __fastcall PacketManager::RecvPacket(int* This)
{
	sS2C_HEADER* pData = (sS2C_HEADER*)This[1];

	//example of usage
	//if (pData->wMainCmd == 0x65)
	//	pData->wMainCmd = 0x00; // set packetid 0 to don't proc game function

	return pData;
}

void PacketManager::SetHooks()
{
	VM_STARTU(__FUNCTION__);
	using namespace Memory;
	HookFunc<HookType::CALL>(SetPacketRegister, 0x0093C04D);
	HookFunc<HookType::CALL>(Rand::CXorKeyTable, 0x006690B6);
	HookFunc<HookType::CALL>(Rand::Generate2ndXorKeyTable, 0x00668A66);
	HookFunc<HookType::CALL>(Rand::DecodeHeader, 0x0093C0EE);
	HookFunc<HookType::CALL>(Rand::DecodePacket, 0x0093C133);
	HookFunc<HookType::CALL>(RecvPacket, 0x0093C148);
	VM_END;
}