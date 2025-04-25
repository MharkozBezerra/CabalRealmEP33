#pragma once
#include "Base.h"

enum MAINCMD_VALUE_EX
{
	// --------------------------------------------------------------------
	MAINCMD_VALUE_EX_BEGIN = 100,
	// --------------------------------------------------------------------

	// --------------------------------------------------------------------
	// FOR_CLIENT_SERVER_COMMUNICATION_BEGIN
	CSC_CONNECT2SVR = 101,	// C2S_CONNECT2SVR, S2C_ACK4CONNECT
	CSC_VERIFYLINKS = 102,	// C2S_VERIFYLINKS, S2C_VERIFYLINKS
	CSC_PREAUTHREQUEST = 103,	// C2S_PREAUTHREQUEST, S2C_PREAUTHREQUEST
	CSC_GETMYCHARTR = 104,	// C2S_GETMYCHARTR, S2C_GETMYCHARTR
	CSC_NEWMYCHARTR = 105,	// C2S_NEWMYCHARTR, S2C_NEWMYCHARTR
	CSC_DELMYCHARTR = 106,	// C2S_DELMYCHARTR, S2C_DELMYCHARTR
	CSC_INITIALIZED = 107,	// C2S_INITIALIZED, S2C_INITIALIZED
	CSC_UNINITIALZE = 108,	// C2S_UNINITIALZE, S2C_UNINITIALZE
	CSC_FDISCONNECT = 109,	// CSC_FDISCONNECT, S2C_FDISCONNECT
	// FOR_CLIENT_SERVER_COMMUNICATION_END
	// --------------------------------------------------------------------

	// --------------------------------------------------------------------
	// FOR_CLIENT_REQUEST_BEGIN
	REQ_DISCONNECTS = 110,	// RES_DISCONNECTS
	// FOR_CLIENT_REQUEST_BEGIN
	// --------------------------------------------------------------------

	// --------------------------------------------------------------------
	// FOR_SERVER_NOTIFY_BEGIN
	NFY_SYSTEMMESSG = 120,	// NFS_SYSTEMMESSG
	// FOR_SERVER_NOTIFY_END
	// --------------------------------------------------------------------

	// --------------------------------------------------------------------
	NFY_SERVERSTATE = 121,	// NFS_SERVERSTATE
	// --------------------------------------------------------------------
	CSC_CHECKVERSION = 122,
	// --------------------------------------------------------------------
	CSC_CHECKINSTANTPLAYTIME = 123,
	CSC_CHANGEUSERPWD = 124,
	CSC_IMAGEAUTHCHECK = 125,
	CSC_IMAGEAUTHDATA = 126,
	// --------------------------------------------------------------------
	ITC_USERCUTOUT = 127,
#ifdef USE_URL_TO_CLIENT
	NFY_URLTOCLIENT = 128,
#endif

	CSC_CUSTOM_PACKET = 421,

	CSC_PUBLICKEY = 2001,
	CSC_PRESERVERENVREQUEST = 2002,
	CSC_LOGINTIMECOUNTDOWN = 2005,
	CSC_AUTHACCOUNT = 2006,
	CSC_LOGINTIMECOUNTDOWN2 = 2009,

	CSC_UNKNOWN5383 = 5383,


	MAINCMD_VALUE_EX_END


	// --------------------------------------------------------------------
};