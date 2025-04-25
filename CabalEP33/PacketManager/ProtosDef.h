#pragma once
#include "Proc/Proc.h"
#include "Base.h"
#include "ProtoDefEx.h"

#define _PROTODEF_VERSION_ 0x0009
#define MAGIC_CODE      (0xB7D9 + _PROTODEF_VERSION_)
#define MAGIC_CODE_EXTENDED  0xC8F3
#pragma pack( push, 1 )

struct sS2C_HEADER
{
	WORD wMagicCode;
	WORD wPayLoadLen;
	WORD wMainCmd;
};

struct sC2S_HEADER
{
	WORD  wMagicCode;
	WORD  wPayLoadLen;
	DWORD dwCheckSum;
	WORD  wMainCmd;

	sC2S_HEADER() :wMagicCode(MAGIC_CODE), wPayLoadLen(0), dwCheckSum(0), wMainCmd(0) {}
	sC2S_HEADER(WORD len, WORD cmd) : wMagicCode(MAGIC_CODE), wPayLoadLen(len), dwCheckSum(0), wMainCmd(cmd) {}
};

struct exS2C_HEADER : public sS2C_HEADER
{
	WORD exMainCmd;
};

struct exC2S_HEADER : public sC2S_HEADER
{
	WORD exMainCmd;
};


struct C2S_CHECKVERSION : public sC2S_HEADER
{
	int						version1,
							version2,
							version3,
							version4;
	C2S_CHECKVERSION() : sC2S_HEADER(sizeof(*this), MAINCMD_VALUE_EX::CSC_CHECKVERSION), version1(0), version2(0), version3(0), version4(0) {}
};

struct S2C_CHECKVERSION : public sS2C_HEADER
{
	int							version;
	int							magickey;
	short						version1;	// 클라이언트 버전
	short						version2;	// Debug용
	short						version3;	// reserved
	short						version4;	// reserved
	S2C_CHECKVERSION() : version(0), magickey(0), version1(0), version2(0), version3(0), version4(0) {}
};

struct sS2C_CHANNELLIST
{
	BYTE ServerID;
	BYTE WorldID;
	WORD PlayerCount;
	WORD LobbyPlayerCount;
	WORD Unknown1;
	WORD CapellaPlayerCount;
	WORD ProcyonPlayerCount;
	DWORD Unknown2;
	WORD CapellaPlayerCount2;
	WORD ProcyonPlayerCount2;
	WORD Unknown3;
	BYTE MinLevel;
	BYTE MaxLevel;
	BYTE MinRank;
	BYTE MaxRank;
	WORD MaxPlayerCount;
	char WorldHost[65];
	WORD WorldPort;
	INT64 WorldType;
};

struct sS2C_SERVERINFO
{
	BYTE  bServerIdx;
	BYTE  bServerType;
	DWORD bGroupData;
	WORD  Unk;
	BYTE  bChannelCount;
};

struct sS2C_SERVERLIST : public sS2C_SERVERINFO
{
	sS2C_CHANNELLIST pS2C_CHANNELLIST[20];
};

struct sS2C_GROUPLIST : public sS2C_HEADER
{
	BYTE  bServerCount;
	sS2C_SERVERLIST* pServerList;
};

struct sS2C_EVENT_ALERT : public exS2C_HEADER
{
	BYTE Event;
	int MapId;
	int Channel;
	int CharIdx;
	INT64 ItemId;
	INT64 ItemOpt;
	char CharName[16];
};

struct sC2S_WINDOWDETECTED : public exC2S_HEADER
{
	char szWindowFinded[256];
	
	sC2S_WINDOWDETECTED(const char* pWindow)
	{
		wMagicCode = MAGIC_CODE;
		wPayLoadLen = sizeof(*this);
		wMainCmd = MAINCMD_VALUE_EX::CSC_CUSTOM_PACKET;
		exMainCmd = MAINCMD_VALUE_CUSTOM::C2S_WINDOWDETECTED;

		strcpy_s(szWindowFinded, sizeof(szWindowFinded), pWindow);
	}
};

#pragma pack( pop )