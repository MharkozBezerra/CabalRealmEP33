#include "Base.h"
#include "Basic.h"
#include "BM3Sinergy/BM3Sinergy.h"
#include "Discord/Discord.h"
#include <sstream>
#include <filesystem>

std::string Basic::Game::GetCharName()
{
	if (USERDATACONTEXT* pUserDataCtx = USERDATACONTEXT::GetpUserDataCtx())
		return (pUserDataCtx->GetCharacterNameLen() < 16 && pUserDataCtx->GetCharacterNameAlloc() < 16 ? pUserDataCtx->GetCharacterName() : reinterpret_cast<char*>(*reinterpret_cast<int*>(pUserDataCtx->GetCharacterName())));

	return std::string("");
}

std::string Basic::Game::GetGuildName()
{
	if (USERDATACONTEXT* pUserDataCtx = USERDATACONTEXT::GetpUserDataCtx())
		return (pUserDataCtx->GetGuildNameLen() < 16 && pUserDataCtx->GetGuildNameAlloc() < 16 ? pUserDataCtx->GetGuildName() : reinterpret_cast<char*>(*reinterpret_cast<int*>(pUserDataCtx->GetGuildName())));

	return std::string("");
}

std::string Basic::Game::GetMapName(USERDATACONTEXT* pUserDataCtx, int MapId, BOOL bUtf8)
{
	typedef int* (__thiscall* TMapInfo)(void*, int);
	TMapInfo MapInfo = (TMapInfo)0x007E401C;

	typedef char* (__thiscall* TMapName)(int*);
	TMapName ObjName = (TMapName)0x0045E520;

	if (int* MapPtr = MapInfo(reinterpret_cast<void*>(0x1139240), MapId ? MapId : GetMapId(pUserDataCtx)))
		return (bUtf8) ? Useful::to_utf8(ObjName(&MapPtr[3])).c_str() : ObjName(&MapPtr[3]);

	return ("Desconhecido");
};

std::string Basic::Game::GetChannelName(int ChannelId)
{
	typedef int(__thiscall* TChannelInfo)(void*, char*, int, int, int*);
	TChannelInfo ChannelInfo = (TChannelInfo)0x0076678A;

	typedef char* (__thiscall* TMapName)(int*);
	TMapName ObjName = (TMapName)0x0045E520;

	if (sChannelInfo* ChannelPtr = new sChannelInfo)
	{
		int FlagPtr;

		ChannelInfo(reinterpret_cast<void*>(0x110E500), reinterpret_cast<char*>(ChannelPtr), *reinterpret_cast<int*>(0x110494C), ChannelId ? ChannelId : GetChannelId(), &FlagPtr);
		return Useful::to_utf8(ObjName(reinterpret_cast<int*>(ChannelPtr))).c_str();

		delete ChannelPtr;
		ChannelPtr = nullptr;
	}

	return std::string("Desconhecido");
}

std::string Basic::Game::GetGameDir()
{
	if (GameDir.empty())
	{
		char* buffer = new(char[MAX_PATH]);

		GetCurrentDirectory(MAX_PATH, buffer);

		GameDir = buffer;

		delete[] buffer;
	}

	return GameDir;
}

bool Basic::Game::EnsureDir(std::string dir)
{
	std::filesystem::path path(GetGameDir());

	path.concat(dir);

	return (!std::filesystem::exists(path) && !std::filesystem::create_directory(path)) ? false : true;
}

INT64 Basic::Game::GetFileSize(std::string filePath)
{
	INT64 fileSize = 0;
	std::string file = GetGameDir() + filePath;

	if (std::filesystem::exists(filePath))
		fileSize = std::filesystem::file_size(file);

	return fileSize;
}

INT64 Basic::Game::GetFileSize(std::string path, std::string filename)
{
	return GetFileSize(path + "\\" + filename);
}

int Basic::Game::LobbyType()
{
	if (sLobbyStatus* pLobby = reinterpret_cast<sLobbyStatus*>(*reinterpret_cast<int*>(0x115928C)))
		return pLobby->Step_1;

	return FALSE;
}

std::string Basic::Useful::to_utf8(const std::string& str, const std::locale& loc) {
	using wcvt = std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t>;
	std::u32string wstr(str.size(), U'\0');
	std::use_facet<std::ctype<char32_t>>(loc).widen(str.data(), str.data() + str.size(), &wstr[0]);
	return wcvt{}.to_bytes(
		reinterpret_cast<const int32_t*>(wstr.data()),
		reinterpret_cast<const int32_t*>(wstr.data() + wstr.size())
	);
}

std::string Basic::Useful::from_utf8(const std::string& str, const std::locale& loc) {
	using wcvt = std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t>;
	auto wstr = wcvt{}.from_bytes(str);
	std::string result(wstr.size(), '0');
	std::use_facet<std::ctype<char32_t>>(loc).narrow(
		reinterpret_cast<const char32_t*>(wstr.data()),
		reinterpret_cast<const char32_t*>(wstr.data() + wstr.size()),
		'?', &result[0]);
	return result;
}

std::string Basic::Useful::ConcatValue(INT64 Value)
{
	std::stringstream ss;
	std::string sValue = std::to_string(Value);
	for (size_t i = 0; i < sValue.length(); i++)
	{
		ss << sValue[i];
		if (((sValue.length() - 1) - i) % 3 == 0 && i != sValue.length() - 1) ss << ",";
	}
	return std::string(ss.str());
}

std::pair<std::string, int> Basic::Useful::getTextAndColor(std::string itemName) {
	std::string text = "";
	int color = 0;

	size_t startPos = itemName.find("$");
	size_t hashPos = itemName.find("#", startPos);
	size_t endPos = itemName.find("$", hashPos);

	if (startPos != std::string::npos && hashPos != std::string::npos && endPos != std::string::npos) {

		color = std::stoi(itemName.substr(startPos + 1, hashPos - startPos - 1));

		text = itemName.substr(hashPos + 1, endPos - hashPos - 1);
	}

	return { text, color };
}

std::string Basic::Useful::FindAndReplace(std::string input, std::string find, std::string replace)
{
	std::size_t pos = input.find(find);
	std::string out = input.replace(pos, find.length(), replace);

	return out;
}

int __fastcall Basic::OnLogged(USERDATACONTEXT* pUserDataCtx, void* n, int Status)
{
	if (pBM3Sinergy)
		pBM3Sinergy->SetPlayerSinergy();

	Discord::SetGameState(Status ? eGAME_STATE::GAME_STATE_PLAYING : eGAME_STATE::GAME_STATE_START);
	pUserDataCtx->SetOnLogged(Status);
	return Status;
}

int Basic::GetVisualConfig(int Type)
{
	return Type != 4 ? static_cast<int>(reinterpret_cast<DWORD*>(0x1184528)[Type]) : 1;
}

void Basic::DlgDelChar(char* Dlg, char* p2)
{
	typedef void(__cdecl* TDlgDelCharF)(char*, char*);
	TDlgDelCharF DlgDelCharF = (TDlgDelCharF)0x008D90B2;

	BYTE* pCharacterSlot = reinterpret_cast<BYTE*>(0x1104C10);
	int CharacterLevel = *(int*)&pCharacterSlot[(*reinterpret_cast<DWORD*>(0x110499C) * 0x428) + 0x44];

	if (CharacterLevel > MAX_DEL_LEVEL)
		return CabalDrawText(reinterpret_cast<void*>(0x00CFA638), std::format("Não é possível deletar um personagem acima do nível {}.", MAX_DEL_LEVEL).c_str(), green_screencenter, 1, -1);

	DlgDelCharF(Dlg, p2);
}

sItem* Basic::Game::Item::GetpItem(INT64 ItemId)
{
	typedef sItem* (__cdecl* TGetpItem)(INT64);
	TGetpItem GetpItem = (TGetpItem)0x00A44623;

	return GetpItem(ItemId & MASK_ITEMKINDINDEX);
}

int Basic::Game::Item::TransformpItem(sItem* pItem, INT64 ItemId, INT64 ItemOpt)
{
	typedef int(__thiscall* TTransformItem)(sItem*, INT64, INT64, int, int, int, int);
	TTransformItem TransformItem = (TTransformItem)0x00A2CB14;

	return TransformItem(pItem, ItemId, ItemOpt, 0, 0, 0, 1);
}

int Basic::Game::Item::GetCodeColor(sItem* pItem)
{
	typedef int(__thiscall* TItemCodeColor)(sItem*, int);
	TItemCodeColor ItemCodeColor = (TItemCodeColor)0x00A2A56A;

	return ItemCodeColor(pItem, 1);
}

char* Basic::Game::Item::GetBasicName(sItem* pItem)
{
	if (sItemInfo* pItemInfo = pItem->psItemInfo)
		return (pItemInfo->ItemNameInfo.ItemNameSize > 15) ? ((sItemDescription*)*(int*)&pItemInfo->ItemNameInfo.ItemAmbiguousName)->ItemContent : pItemInfo->ItemNameInfo.ItemAmbiguousName;

	return nullptr;
}

char* Basic::Game::Item::GetCompleteName(sItem* pItem)
{
	typedef sItemTransformName* (__thiscall* TGetCompleteItemName)(sItem*, sItemTransformName*);
	TGetCompleteItemName GetCompleteItemName = (TGetCompleteItemName)0x00A2B681;

	if (std::shared_ptr<sItemTransformName> pItemName = std::make_shared<sItemTransformName>())
	{
		GetCompleteItemName(pItem, pItemName.get());
		return (pItemName->AllocNameSize < 16 ? pItemName->Name : reinterpret_cast<char*>(*reinterpret_cast<int*>(pItemName->Name)));
	}

	return nullptr;
}

char* Basic::Game::Item::GetFullName(sItem* pItem, BOOL bColor)
{
	typedef sItemTransformName* (__thiscall* TTransformName)(sItem*, sItemTransformName*, int);
	TTransformName TransformName = (TTransformName)0x00A2BC80;

	if (std::shared_ptr<sItemTransformName> pItemName = std::make_shared<sItemTransformName>())
	{
		TransformName(pItem, pItemName.get(), bColor);
		return (pItemName->AllocNameSize < 16 ? pItemName->Name : reinterpret_cast<char*>(*reinterpret_cast<int*>(pItemName->Name)));
	}

	return nullptr;
}

void Basic::Game::Item::MoveItemToUi()
{
	VM_STARTV(__FUNCTION__);
	typedef void(__thiscall* t_MovItemToUI)(void*, sPutItemOnUI*);
	typedef sCabalWindow* (__thiscall* T_CALL)(void*, int);
	T_CALL WindowStatus = (T_CALL)0x00917F6C, ParentWindow = (T_CALL)0x007F85C7;
	sCabalWindow* pInventoryWindow	= WindowStatus(reinterpret_cast<void*>(0x115AF90), 24500000);
	sCabalWindow* pCabalWindow		= WindowStatus(reinterpret_cast<void*>(0x115AF90), 56500000);

	if (auto* pUserDataContext = USERDATACONTEXT::GetpUserDataCtx())
	{
		if (pUserDataContext->GetOnLogged() && !pUserDataContext->getCursorItem())
		{
			if (pCabalWindow && pInventoryWindow)
			{
				if (!pCabalWindow->GetWindowStat() || !pInventoryWindow->GetWindowStat())
					return;

				int*** pUiData = pUserDataContext->GetUiInfomation();
				if (*pUiData)
				{
					t_MovItemToUI MovItemToUI = (t_MovItemToUI) * **pUiData;

					int InventoryTab = pInventoryWindow->GetInventoryTabIdx();
					int inventoryStart = (E_START_INV_SLOT + (MAX_INVENTORYTAB * InventoryTab));

					for (int slot = inventoryStart; slot < inventoryStart + 64; slot++)
						if (pUserDataContext->GetpSlotItem(slot))
							MovItemToUI(*pUiData, &sPutItemOnUI(slot, 24500000));
				}
			}
		}
	}
	VM_END;
}

void Basic::SetHooks()
{
	VM_STARTU(__FUNCTION__);
	using namespace Memory;
	//VisualGame Config
	HookFunc<HookType::JMP>(GetVisualConfig, 0x00A0775C);
	HookFunc<HookType::CALL>(OnLogged, { 0x00950535, 0x007000F5 });
	WriteValue<DWORD>(0x091B24F, reinterpret_cast<DWORD>(DlgDelChar));
	VM_END;
}
