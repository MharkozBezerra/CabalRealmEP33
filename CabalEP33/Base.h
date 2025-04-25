#pragma once
#include <Windows.h>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <map>

#include "3rdparty/VMProtect/VMProtectSDK.h"

#define DEV_MODE
#define XIGNCODE_VERSION_NA
//#define XIGNCODE_LOAD_X3_XEM

#pragma region
#define VM_START(Var)		VMProtectBegin(Var)
#define VM_STARTV(Var)		VMProtectBeginVirtualization(Var)
#define VM_STARTM(Var)		VMProtectBeginMutation(Var)
#define VM_STARTU(Var)		VMProtectBeginUltra(Var)
#define VM_END				VMProtectEnd()
#pragma endregion VIRTUALIZACAO