#pragma once
#include "Base.h"
#include "Proc/Proc.h"
#include "../../Memory/Memory.h"
#include "../UserDataContextMapping/UserDataContext.h"
#include "PacketManager/ProtoDefEx.h"
#include "../MyDraw/Objects.h"

enum eATTACKSBM3 {
	COMBO_FATAL0_ATTACKID0 = 0x00000000,
	COMBO_FATAL0_ATTACKID1 = 0x00000001,
	COMBO_FATAL0_ATTACKID2 = 0x00000002,
	COMBO_FATAL1_ATTACKID0 = 0x00010000,
	COMBO_FATAL1_ATTACKID1 = 0x00010001,
	COMBO_FATAL1_ATTACKID2 = 0x00010002,
	COMBO_FATAL2_ATTACKID0 = 0x00020000,
	COMBO_FATAL2_ATTACKID1 = 0x00020001,
	COMBO_FATAL2_ATTACKID2 = 0x00020002,
	COMBO_FATAL3_ATTACKID0 = 0x00030000,
	COMBO_FATAL3_ATTACKID1 = 0x00030001,
	COMBO_FATAL3_ATTACKID2 = 0x00030002
};

enum BM3_ATTACKNAMES
{
	ATK_A = 0,
	ATK_B,
	ATK_FATAL1,
	ATK_FATAL2,
	ATK_FATAL3,
};

#pragma pack(push,1)
struct S2C_SKILLFROMUSER
{
	char cS2CH[6];
	unsigned __int16 skillIdx;
	unsigned __int32 characterIdx;
	unsigned __int64 tickCount;
	unsigned __int8 styleEx;
	unsigned __int16 enabled;
};

struct C2S_BM3MACROREQUEST : C2S_HEADER_EX
{
	bool status;

	void Initialize(bool value)
	{
		wMagicCode = 0xB7E2;
		wPayloadLen = sizeof(C2S_BM3MACROREQUEST);
		wMainCmd = MAINCMD_VALUE_EX::CSC_CUSTOM_PACKET;
		wMainCmdEx = MAINCMD_VALUE_CUSTOM::S2C_BM3MACRORESULT;
		status = value;
	}
};

struct S2C_BM3MACRORESPONSE : S2C_HEADER_EX
{
	int result;
};

struct S2C_RECEIVESTYLEEX : S2C_HEADER
{
	unsigned __int64 myHp;
	unsigned __int32 myMp;
	unsigned __int64 myShieldHP;
	unsigned __int32 battlemodeRemainTime;
	unsigned __int8 battleModeIdx;
	unsigned __int8 bUnk1;
	unsigned __int64 bUnk0;
	unsigned __int32 iUnk0;
	unsigned __int32 iUnk1;
	unsigned __int16 iUnk2;
};


struct NFY_SKILLTOUSER : S2C_HEADER
{
	WORD wSkillIdx;
	DWORD dwCharacterIdx;
	DWORD dwStyle;
	WORD wLiveStyle;
	BYTE unk1;
	BYTE unk2;
	BYTE bBattleModeIdx;
	BYTE bIsUsingBm;
	BYTE unk3;
};


struct S_BUTTONS_SEQUENCES 
{
	POINT uiPosition = { 0,0 };
	int fatalCount = 0;
	int btnVisibility[4][3] = { 0,{0} };
	int combo[3] = { -1 };
};

struct S_MACROBM3_DATA 
{
	struct SinergyData 
	{
		BYTE Attack[4] = { 0 };

		bool IsValid()
		{
			return *(DWORD*)Attack ? true : false;
		}
	};

	SinergyData sinergyData[4];

	S_MACROBM3_DATA()
	{
		memset(sinergyData, 0, sizeof(S_MACROBM3_DATA));
	}
};
#pragma pack(pop)


class BM3Macro : public CustomDraw
{
public:
	virtual void NotifyDevice();
	virtual void Present();
	virtual void Draw();
	virtual void Release();
	virtual LRESULT CALLBACK OnKeyBoardEvent(WPARAM wParam, WPARAM wParamTranslated);
	virtual LRESULT CALLBACK MouseClick(WINPROC_CLICKIDX state);
	virtual LRESULT CALLBACK MouseMove(WPARAM wParam);
	virtual bool IsVisible() { return this->ObjVisible; };
	virtual void SaveOnClose();
	virtual void ResizeWindow();
	/*virtual void CabalParentEvent(CabalParentData* parentPtr, BOOL& isVisible);*/

	bool ObjVisible = false;

	std::vector<cSzObjectContext*> GetVectorObjects() { return vObjects; };


	/********************************************************/

	std::vector<cSzObjectContext*> vObjects;
	std::map<int, S_MACROBM3_DATA> m_BM3Data;
	std::map<int, std::string> forceNames;
	S_BUTTONS_SEQUENCES sComboData;
	std::string macroPath = "\\Data\\UserData\\BM3Macro";

	bool pause					= false;
	double timeActNextTime		= 0;

	//flag que indica se os objetos ja foram carregados
	bool isInitialized = false;


	/********************************************************/
	//objects
	cSzObjectContext obj_MainBoxMaximized;
	cSzObjectContext obj_MainBoxMinimized;
	cSzObjectContext obj_AddImages[5];

	//buttons
	cSzObjectContext obj_MaximizeBtn;
	cSzObjectContext obj_MinimizeBtn;

	//radio buttons
	cSzObjectContext obj_RadioButton[4][3];
	cSzObjectContext obj_RadioButton_Fatal[3];


	cSzObjectContext txt_WindowName;
	cSzObjectContext txt_Descriptions[4];
	cSzObjectContext txt_SequenceName[4];
	cSzObjectContext txt_SinergyIndex[4][3];
	cSzObjectContext txt_FatalIndex[3];


	//textbox
	/*******************************************/
	int MAX_SYNERGY_COUNT = 3;
	int MAX_SEQUENCES_COUNT = 4;

	//valor padrão inicial para as sequancias
	const int BM3_MAX_SEQUENCES = 1;

	//é alterada para true para permitir o uso do fatal em determinados momentos
	bool useFatal = false;

	//numero inicial de sequencia que sempre deve ser padrão = 1
	int currentSequences = BM3_MAX_SEQUENCES;

	//quantidade de fatais a serem utilizadas padrão = 0
	int currentFatalCount = 0;

	//numero atual de hits dados 0 ~ 3
	int hitCount = 0;

	//quantidade de sequencias a serem executadas
	int sequences = 0;

	void AttackManager();

	static void MinimizeWindow(cSzObjectContext* a, int b, void* c, WINPROC_CLICKIDX clickType);
	static void MaximizeWindow(cSzObjectContext* a, int b, void* c, WINPROC_CLICKIDX clickType);

	void MountDescription();

	void WindowManager(bool status);

	//trata a seleção de dos radio buttons dos fatais
	static void OnSelectSynergyOptions(cSzObjectContext* pObjectCtx, int b, void* c, WINPROC_CLICKIDX clickType);
	static void OnSelectFatalOptions(cSzObjectContext* pObjectCtx, int slot, void*, WINPROC_CLICKIDX clickType);

	void ResetAttacks() 
	{
		useFatal = false;
		hitCount = 0;
		currentFatalCount = 0;
		currentSequences = BM3_MAX_SEQUENCES;
	};

	void LoadJson();
	void Init();

	//pacote ao entrar no personagem
	static int __cdecl InitializeStyleEX(S2C_RECEIVESTYLEEX* packet);
	static int __cdecl NfySkillToUser(NFY_SKILLTOUSER* packet);
	static void OnS2CBM3MacroResponse(S2C_HEADER_EX* pS2CHeader, int size, void* unk);

	void LoadConfigFile(std::string filePath, void* buffer, size_t buflen);
	void SaveConfigFile(std::string filePath, void* buffer, size_t buflen);
};


extern std::shared_ptr<BM3Macro> g_pBM3Macro;