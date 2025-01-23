#include <Windows.h>
#include "minhook/MinHook.h"
#include "Utils.h"
#include "NetDriver.h"
#include "Gamemode.h"
#include "World.h"
#include "Misc.h"
#include "PC.h"
#include <cstdio>

#pragma comment(lib, "minhook/minhook.lib")

using namespace std;
using namespace SDK;

DWORD InitThread(LPVOID)
{
    srand(time(0));
    
    AllocConsole();
    FILE* console;
    freopen_s(&console, "CONOUT$", "w", stdout);
    MH_Initialize();
    InitGObjects();
    Utils::Log("Initializing...");

    Sleep(15000);

    Gamemode::InitHooks();
    NetDriver::InitHooks();
    World::InitHooks();
    PC::InitHooks();
    Misc::InitHooks();
    Abilities::InitHooks();
    
    *(bool*)(__int64(GetModuleHandleW(0)) + 0x6F41270) = false;
    *(bool*)(__int64(GetModuleHandleW(0)) + 0x6F41271) = true;

    SpectatingName = UKismetStringLibrary::GetDefaultObj()->Conv_StringToName(TEXT("Spectating"));
    ((UGameplayStatics*)UGameplayStatics::StaticClass()->DefaultObject)->OpenLevel(Utils::GetWorld(), ((UKismetStringLibrary*)UKismetStringLibrary::StaticClass()->DefaultObject)->Conv_StringToName(TEXT("Apollo_Terrain")), true, FString());

    Utils::GetEngine()->GameInstance->LocalPlayers.FreeArray();
    
    Sleep(1500);
    Gamemode::Ready = true;
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD Reason, LPVOID lpReserved)
{
    switch (Reason)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, InitThread, 0, 0, 0);
        break;
    default:
        break;
    }
    return TRUE;
}