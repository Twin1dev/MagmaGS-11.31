#include "NetDriver.h"

#include "Gamemode.h"
#include "minhook/MinHook.h"

#pragma comment(lib, "minhook/minhook.lib")

using namespace SDK;
using namespace Params;
using namespace std;

void (*TickFlushOG)(UNetDriver* Driver, float a2);

void NetDriver::InitHooks()
{
	static auto Base = __int64(GetModuleHandleW(0));
	Utils::Log("Initializing NetDriver Hooks...");

	MH_CreateHook((LPVOID)(Base + 0x3883CD0), NetDriver::TickFlush, (LPVOID*)&TickFlushOG);
	MH_EnableHook((LPVOID)(Base + 0x3883CD0));
	MH_CreateHook((LPVOID)(Base + 0x372FB80), NetDriver::kick, nullptr);
	MH_EnableHook((LPVOID)(Base + 0x372FB80));
}

void NetDriver::TickFlush(UNetDriver* Driver, float a2)
{
	static void (*ServerReplicateActors)(void*) = decltype(ServerReplicateActors)((*(void***)UFortReplicationGraph::StaticClass()->DefaultObject)[0x59]);
	if (ServerReplicateActors && Driver->ReplicationDriver)
		ServerReplicateActors(Driver->ReplicationDriver);
	TickFlushOG(Driver, a2);

	if (BotsEnabled)
	{
		static bool First = false;
		static bool First2 = false;
		static TArray<AActor*> PlayerStarts;
		auto GameMode = (AFortGameModeAthena*)Driver->World->AuthorityGameMode;
		auto GameState = (AFortGameStateAthena*)Driver->World->GameState;

		if (!First)
		{
			UGameplayStatics::GetDefaultObj()->GetAllActorsOfClass(UWorld::GetWorld(), AFortPlayerStartWarmup::StaticClass(), &PlayerStarts);
			First = true;
		}

		if (((AFortGameStateAthena*)UWorld::GetWorld()->GameState)->GamePhase < EAthenaGamePhase::Aircraft && GameMode->AlivePlayers.Num() > 0 && (GameMode->AlivePlayers.Num() + GameMode->AliveBots.Num()) < 100)
		{
			GameState->WarmupCountdownEndTime = UGameplayStatics::GetDefaultObj()->GetTimeSeconds(UWorld::GetWorld()) + 12;

			if (UKismetMathLibrary::GetDefaultObj()->RandomBoolWithWeight(0.07f))
			{
				AActor* SpawnLocator = PlayerStarts[UKismetMathLibrary::GetDefaultObj()->RandomIntegerInRange(0, PlayerStarts.Num() - 1)];

				if (SpawnLocator)
				{
					Utils::Log("Spawning New Bot");
					SpawnedBots.push_back(new Bot(SpawnLocator));
				}
			}
		}

		if (!First2 && GameMode->AlivePlayers.Num() > 0 && GameState->GamePhase == EAthenaGamePhase::Warmup && GameState->WarmupCountdownEndTime - UGameplayStatics::GetDefaultObj()->GetTimeSeconds(UWorld::GetWorld()) <= 0)
		{
			First2 = true;
			UKismetSystemLibrary::GetDefaultObj()->ExecuteConsoleCommand(UWorld::GetWorld(), TEXT("startaircraft"), nullptr);
		}

		if (BotsEnabled)
		{
			for (size_t i = 0; i < SpawnedBots.size(); i++)
			{
				SpawnedBots[i]->Tick();
			}
		}
	}
}

char __fastcall NetDriver::kick(__int64, __int64, __int64)
{
	return 0;
}