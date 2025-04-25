#include "Base.h"
#include "Proc.h"
#include "Memory/Memory.h"

std::shared_ptr<Proc> g_Procedures = std::make_shared<Proc>();

void ExecCustomPacket(S2C_HEADER_EX* pS2CHeader, size_t size, void* unk)
{
	VM_STARTM(__FUNCTION__);

	if (g_Procedures->m_procedures.contains(pS2CHeader->wMainCmdEx))
	{
		for (auto& it : g_Procedures->m_procedures[pS2CHeader->wMainCmdEx])
		{
			it(pS2CHeader, size, unk);
		}
	}

	VM_END;
}

void Proc::RegisterProcedure(MAINCMD_VALUE_CUSTOM customCmd, std::function<void(S2C_HEADER_EX*, int, void*)> function)
{
	VM_STARTM(__FUNCTION__);
	m_procedures[customCmd].push_back(function);
	VM_END;
}

void Proc::SetHooks()
{
	VM_STARTU(__FUNCTION__);
	//execute custom packet
	Memory::WriteValue(0x0095C2C4, ExecCustomPacket);
	VM_END;
}
