#pragma once
#include "Base.h"
#include <vector>
#include <map>
#include <format>
#include <intrin.h>
#include <functional>

enum MAINCMD_VALUE_CUSTOM
{
	S2C_EVENT_ALERT, 
	S2C_BM3MACRORESULT,
	C2S_WINDOWDETECTED
};

#pragma pack(push,1)
struct S2C_HEADER
{
	WORD wMagicCode;
	WORD wPayloadLen;
	WORD wMainCmd;
};

struct S2C_HEADER_EX
{
	WORD wMagicCode;
	WORD wPayloadLen;
	WORD wMainCmd;
	WORD wMainCmdEx;
};

struct C2S_HEADER_EX
{
	WORD wMagicCode;
	WORD wPayloadLen;
	DWORD dwCheckSum;
	WORD wMainCmd;
	WORD wMainCmdEx;
};

struct S2C_PUBLICKEY : S2C_HEADER
{
	BYTE status;
	WORD size;
	BYTE publicKey[1];
};
#pragma pack(pop)

class Proc
{
public:

	std::unordered_map<int, std::vector<std::function<void(S2C_HEADER_EX*, int, void*)>>> m_procedures; //[extcmd][functions]

	void RegisterProcedure(MAINCMD_VALUE_CUSTOM customCmd, std::function<void(S2C_HEADER_EX*, int, void*)> function);

	void SetHooks();
};

extern std::shared_ptr<Proc> g_Procedures;

#define REGISTERCUSTOMPROC(ext_cmd,function)				g_Procedures->RegisterProcedure(ext_cmd,function)