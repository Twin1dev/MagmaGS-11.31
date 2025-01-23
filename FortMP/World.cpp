#include "World.h"
#include "minhook/MinHook.h"

#pragma comment(lib, "minhook/minhook.lib")

using namespace SDK;

BYTE* __fastcall ChangeGameSessionId(__int64, __int64)
{
	return 0;
}

void World::InitHooks()
{
	static auto Base = __int64(GetModuleHandleW(0));
	Utils::Log("Initializing World Hooks...");
	
	MH_CreateHook((LPVOID)(Base + 0x3B76FE0), World::GetNetMode, nullptr);
	MH_EnableHook((LPVOID)(Base + 0x3B76FE0));
	MH_CreateHook((LPVOID)(Base + 0x34AF6C0), World::GetNetMode, nullptr);//actor get net mode
	MH_EnableHook((LPVOID)(Base + 0x34AF6C0));
	MH_CreateHook((LPVOID)(Base + 0x19B1660), ChangeGameSessionId, nullptr);
	MH_EnableHook((LPVOID)(Base + 0x19B1660));
}

__int64 World::GetNetMode(void*)
{
	return 1;
}