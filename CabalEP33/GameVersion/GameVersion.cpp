#include "Base.h"
#include "GameVersion.h"
#include "PacketManager/PacketManager.h"
#include "Memory/Memory.h"

#ifdef USE_CUSTOM_VERSION
void GameVersion::SendGameVersion()
{
	VM_STARTV(__FUNCTION__);
	C2S_CHECKVERSION Packet;
	Packet.version1 = GAMEVERSION;

#ifdef _DEBUG_MODE
	Packet.version1 = GAMEVERSION;
#endif

	PacketManager::Send(eGameSocket::Login, &Packet, sizeof(C2S_CHECKVERSION));
	VM_END;
}

int __fastcall GameVersion::RecvGameVersion(void* This)
{
	VM_STARTV(__FUNCTION__);
	return GAMEVERSION;
	VM_END;
}
#endif

#ifdef USE_CUSTOM_MAGICKEY
int __fastcall GameVersion::GetMagicKey(void* This)
{
	VM_STARTV(__FUNCTION__);
	return GAMEMAGICKEY;
	VM_END;
}
#endif

#ifdef USE_CUSTOM_XORKEY
DWORD GameVersion::DecryptXOR(DWORD* pdw)
{
	VM_STARTM(__FUNCTION__);
	if (pdw[3] > pdw[2]) return -1;

	DWORD iPos = pdw[3];
	if (iPos < sizeof(vXorKeys))
		return *(BYTE*)(pdw[1] + iPos) ^ vXorKeys[pdw[3]++];

	return *(BYTE*)(pdw[1] + pdw[3]++);
	VM_END;
}
#endif

void GameVersion::SetHooks()
{
	VM_STARTU(__FUNCTION__);
	using namespace Memory;

#ifdef USE_CUSTOM_VERSION
	HookFunc<HookType::CALL>(RecvGameVersion, 0x0093EFEB);
	WriteValue<DWORD>(0x0093D165, reinterpret_cast<DWORD>(GameVersion::SendGameVersion));
#endif

#ifdef USE_CUSTOM_MAGICKEY
	HookFunc<HookType::CALL>(GetMagicKey, { 0x00666FD8, 0x0093FD03 });
#endif

#ifdef USE_CUSTOM_XORKEY
	HookFunc<HookType::CALL>(DecryptXOR, { 0x006697DE, 0x00669807, 0x0066989E, 0x006698E4, 0x0066997F, 0x006699BD, 0x00669A03,
										   0x00669A4E, 0x00669B5E, 0x00669B91, 0x00669BC3, 0x00669C19, 0x00669CDF, 0x00669D3B,
										   0x00669D89, 0x00669DB7, 0x00669FF7, 0x0066A026, 0x0066A063 });
#endif
	VM_END;
}
