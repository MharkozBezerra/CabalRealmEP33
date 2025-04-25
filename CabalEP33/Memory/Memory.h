#pragma once
#include "Base.h"
#include <boost/bind.hpp>

namespace Memory
{
	enum HookType
	{
		NONE,
		JMP,
		CALL,
		JMP_LONG,
		CALL_LONG,
		VIRTUAL_JMP_LONG,
		VIRTUAL_CALL_LONG,
	};

	struct sHookType
	{
		HookType Type;
		WORD OpCode;
		BOOL bLong;
		BOOL bVirtual;
		DWORD dwSize;
		DWORD dwDistSize;
	};

	static std::vector<sHookType> vHookType =
	{
		{HookType::JMP,                 0xE9,   0, 0, 5, 1},
		{HookType::CALL,                0xE8,   0, 0, 5, 1},
		{HookType::JMP_LONG,            0x25FF, 1, 0, 6, 2},
		{HookType::CALL_LONG,           0x15FF, 1, 0, 6, 2},
		{HookType::VIRTUAL_JMP_LONG,    0xE9,   0, 1, 5, 1},
		{HookType::VIRTUAL_CALL_LONG,   0xE8,   0, 1, 5, 1},
	};

	static void SampleHookFunc(DWORD dwNewFunc, DWORD dwOldAddr)
	{
		DWORD Old;

		VirtualProtect((void*)dwOldAddr, 5, PAGE_EXECUTE_READWRITE, &Old);
		*(BYTE*)(dwOldAddr) = 0xE9;
		*(DWORD*)(dwOldAddr + 1) = dwNewFunc - dwOldAddr - 5;
		VirtualProtect((void*)dwOldAddr, 5, Old, &Old);
	}

	template <typename T, typename TAddr>
	void SetMemory(TAddr dwAddr, std::vector<T> vNewMem, DWORD dwPermission = PAGE_EXECUTE_READWRITE)
	{
		DWORD Old = 0;
		DWORD dwSize = vNewMem.size() * sizeof(T);
		VirtualProtect((void*)dwAddr, dwSize, dwPermission, &Old);
		memcpy((void*)dwAddr, vNewMem.data(), dwSize);
		VirtualProtect((void*)dwAddr, dwSize, Old, &Old);
	}

	template <typename T, typename TAddr>
	void SetMemory(std::vector<TAddr> vAddr, std::vector<T> vNewMem, DWORD dwPermission = PAGE_EXECUTE_READWRITE)
	{
		for (auto dwAddr : vAddr)
			SetMemory<T, TAddr>(dwAddr, vNewMem, dwPermission);
	}

	template <HookType Type, typename Func>
	void HookFunc(Func dwNewFunc, DWORD dwOldAddr, DWORD dwPermission = PAGE_EXECUTE_READWRITE)
	{
		auto it = std::find_if(vHookType.begin(), vHookType.end(), boost::bind(&sHookType::Type, _1) == Type);
		if (it != vHookType.end())
		{
			DWORD Old = 0;
			BYTE* pVirtualFunc = nullptr;
			DWORD* pLongFunc = nullptr;

			if (it->bVirtual)
				if (pVirtualFunc = new BYTE[5])
					SampleHookFunc((DWORD)dwNewFunc, (DWORD)pVirtualFunc);

			if (it->bLong)
				if (pLongFunc = new DWORD)
					*pLongFunc = (DWORD)dwNewFunc;

			VirtualProtect((void*)dwOldAddr, it->dwSize, dwPermission, &Old);
			memcpy((void*)dwOldAddr, &it->OpCode, it->dwDistSize);
			*(DWORD*)(dwOldAddr + it->dwDistSize) = (pLongFunc) ? (DWORD)pLongFunc : (((pVirtualFunc) ? (DWORD)pVirtualFunc : (DWORD)dwNewFunc) - dwOldAddr - 5);
			VirtualProtect((void*)dwOldAddr, it->dwSize, Old, &Old);
		}
	}

	template<typename TValue>
	__declspec(noinline)void WriteValue(DWORD Addr, TValue Valor)
	{
		DWORD OldProtect;
		if (!VirtualProtect((void*)Addr, sizeof(TValue), PAGE_EXECUTE_READWRITE, &OldProtect)) return;
		*(TValue*)Addr = Valor;
		VirtualProtect((void*)Addr, sizeof(TValue), OldProtect, &OldProtect);
	}

	template <HookType Type, typename Func>
	void HookFunc(Func dwNewFunc, std::vector<DWORD> vOldAddr, DWORD dwPermission = PAGE_EXECUTE_READWRITE)
	{
		for (auto dwOldAddr : vOldAddr)
			HookFunc<Type>(dwNewFunc, dwOldAddr, dwPermission);
	}

	template <typename Func>
	void Fill(Func dwAddr, int Value, size_t szLen)
	{
		DWORD Old = 0;

		VirtualProtect((void*)dwAddr, szLen, PAGE_EXECUTE_READWRITE, &Old);
		memset((void*)dwAddr, Value, szLen);
		VirtualProtect((void*)dwAddr, szLen, Old, &Old);
	}
}