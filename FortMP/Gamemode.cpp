#include "Gamemode.h"
#include "minhook/MinHook.h"
#include <iostream>
#include <fstream>
#include "Misc.h"

#pragma comment(lib, "minhook/minhook.lib")

using namespace std;
using namespace SDK;
using namespace Params;

bool (*ReadyToStartMatchOG)(SDK::AFortGameModeAthena* GM);
bool Gamemode::ReadyToStartMatch(SDK::AFortGameModeAthena* GM)
{
	if (IsSTW)
	{
		static bool Done2 = false;
		AFortGameModeOutpost* GameMode = (AFortGameModeOutpost*)GM;
		if (!Ready || !GM->GameState)
			return false;
		GameMode->bTheaterDataIsReady = true;
		AFortGameStateOutpost* GameState = (AFortGameStateOutpost*)GM->GameState;
		GameState->bOutpostDefenseActive = true;

		if (!Done2)
		{
			Done2 = true;
			auto Engine = Utils::GetEngine();
			auto World = Utils::GetWorld();
			FName Name = ((UKismetStringLibrary*)UKismetStringLibrary::StaticClass()->DefaultObject)->Conv_StringToName(TEXT("GameNetDriver"));
			UNetDriver* Driver = CreateNetDriver(Engine, World, Name);

			Driver->World = World;
			Driver->NetDriverName = Name;

			FString Error;
			FURL InURL = FURL();
			InURL.Port = 7777;

			InitListen(Driver, World, InURL, false, Error);
			SetWorld(Driver, World);

			World->NetDriver = Driver;
			World->LevelCollections[0].NetDriver = Driver;
			World->LevelCollections[1].NetDriver = Driver;
		}

		return ReadyToStartMatchOG(GM);
	}
	else
	{
		ReadyToStartMatchOG(GM);
		auto GameState = (AFortGameStateAthena*)GM->GameState;
		static bool Done = false;
		static bool Done2 = false;
		if (!Ready || !GameState)
			return false;
		if (!Done)
		{
			Done = true;
			UFortPlaylistAthena* Playlist = IsDadBro ? Utils::StaticFindObject<UFortPlaylistAthena>("/Game/Athena/Playlists/DADBRO/Playlist_DADBRO_Squads.Playlist_DADBRO_Squads") : Utils::StaticFindObject<UFortPlaylistAthena>("/Game/Athena/Playlists/Playlist_DefaultSolo.Playlist_DefaultSolo");
			GameState->CurrentPlaylistInfo.BasePlaylist = Playlist;
			GameState->CurrentPlaylistInfo.PlaylistReplicationKey++;
			GameState->CurrentPlaylistId = Playlist->PlaylistId;

			GameState->CurrentPlaylistInfo.MarkArrayDirty();
			NextIdx = Playlist->DefaultFirstTeam;
			MaxPlayersOnTeam = Playlist->MaxSquadSize;

			// skunked!

			bool bDBNO = MaxPlayersOnTeam > 1;

			GameState->bDBNOEnabledForGameMode = bDBNO;
			GameState->bDBNODeathEnabled = bDBNO;

			GM->bAlwaysDBNO = bDBNO;
			GM->bDBNOEnabled = bDBNO;

			GM->CurrentPlaylistId = Playlist->PlaylistId;
			GM->CurrentPlaylistName = Playlist->PlaylistName;

			LootTierData = Utils::StaticLoadObject<UDataTable>(UKismetStringLibrary::GetDefaultObj()->Conv_NameToString(Playlist->LootTierData.ObjectID.AssetPathName).ToString());
			LootPackages = Utils::StaticLoadObject<UDataTable>(UKismetStringLibrary::GetDefaultObj()->Conv_NameToString(Playlist->LootPackages.ObjectID.AssetPathName).ToString());

			cout << LootTierData->GetName() << endl;
			cout << LootPackages->GetName() << endl;
		}

		if (!GameState->MapInfo)
			return false;

		if (!Done2)
		{
			Done2 = true;
			GameState->OnRep_CurrentPlaylistId();
			GameState->OnRep_CurrentPlaylistInfo();

			auto Engine = Utils::GetEngine();
			auto World = Utils::GetWorld();
			FName Name = ((UKismetStringLibrary*)UKismetStringLibrary::StaticClass()->DefaultObject)->Conv_StringToName(TEXT("GameNetDriver"));
			UNetDriver* Driver = CreateNetDriver(Engine, World, Name);

			Driver->World = World;
			Driver->NetDriverName = Name;

			FString Error;
			FURL InURL = FURL();
			InURL.Port = 7777;

			InitListen(Driver, World, InURL, false, Error);
			SetWorld(Driver, World);

			World->NetDriver = Driver;
			World->LevelCollections[0].NetDriver = Driver;
			World->LevelCollections[1].NetDriver = Driver;

			//for (size_t i = 0; i < GameState->CurrentPlaylistInfo.BasePlaylist->AdditionalLevels.Num(); i++)
			//{
			//	bool Success = false;
			//	ULevelStreamingDynamic::GetDefaultObj()->LoadLevelInstanceBySoftObjectPtr(UWorld::GetWorld(), GameState->CurrentPlaylistInfo.BasePlaylist->AdditionalLevels[i], FVector(), FRotator(), &Success);
			//	GameState->AdditionalPlaylistLevelsStreamed.Add(GameState->CurrentPlaylistInfo.BasePlaylist->AdditionalLevels[i].ObjectID.AssetPathName);
			//	Utils::Log(GameState->CurrentPlaylistInfo.BasePlaylist->AdditionalLevels[i].ObjectID.AssetPathName.ToString());
			//}

			//GameState->OnRep_AdditionalPlaylistLevelsStreamed();

			if (BotsEnabled)
			{
				BotMutator = Utils::SpawnActor<AFortAthenaMutator_Bots>({}, {}, nullptr, Utils::StaticLoadObject<UClass>("/Game/Athena/AI/Phoebe/BP_Phoebe_Mutator.BP_Phoebe_Mutator_C"));
				BotMutator->CachedGameMode = GM;
				BotMutator->CachedGameState = GameState;

				GM->ServerBotManager = (UFortServerBotManagerAthena*)UGameplayStatics::GetDefaultObj()->SpawnObject(UFortServerBotManagerAthena::StaticClass(), GM);
				GM->ServerBotManager->CachedBotMutator = BotMutator;
				GM->ServerBotManager->CachedGameMode = GM;
				GM->ServerBotManager->CachedGameState = GameState;

				GM->AIDirector = Utils::SpawnActor<AAthenaAIDirector>({});
				GM->AIDirector->Activate();
			}

			TArray<AActor*> VehicleSpawners;
			UGameplayStatics::GetDefaultObj()->GetAllActorsOfClass(UWorld::GetWorld(), AFortAthenaVehicleSpawner::StaticClass(), &VehicleSpawners);

			UClass* MeatballClass = Utils::StaticLoadObject<UClass>("/Game/Athena/DrivableVehicles/Meatball/Meatball_Large/MeatballVehicle_L.MeatballVehicle_L_C");

			Utils::SwapVTable(Utils::StaticLoadObject<UClass>("/Game/Athena/DrivableVehicles/Meatball/Meatball_Large/MeatballVehicle_L.MeatballVehicle_L_C")->DefaultObject, 0xED, Misc::ServerMove);

			for (size_t i = 0; i < VehicleSpawners.Num(); i++)
			{
				auto Vehicle = VehicleSpawners[i];
				if (((AFortAthenaVehicleSpawner*)Vehicle)->GetVehicleClass() == MeatballClass)
				{
					Utils::SpawnActor<AFortAthenaVehicle>(Vehicle->K2_GetActorLocation(), Vehicle->K2_GetActorRotation(), nullptr, ((AFortAthenaVehicleSpawner*)Vehicle)->GetVehicleClass());
				}
			}

			VehicleSpawners.FreeArray();

			CIDs = Utils::GetAllObjectsOfClass<UAthenaCharacterItemDefinition>();
			Pickaxes = Utils::GetAllObjectsOfClass<UAthenaPickaxeItemDefinition>();
			Backpacks = Utils::GetAllObjectsOfClass<UAthenaBackpackItemDefinition>();
			Gliders = Utils::GetAllObjectsOfClass<UAthenaGliderItemDefinition>();
			Contrails = Utils::GetAllObjectsOfClass<UAthenaSkyDiveContrailItemDefinition>();
			Dances = Utils::GetAllObjectsOfClass<UAthenaDanceItemDefinition>();

			GM->bWorldIsReady = true;
			cout << GameState->MapInfo->SupplyDropInfoList.Num() << endl;

			GameState->MapInfo->SupplyDropInfoList[0]->SupplyDropClass = Utils::StaticLoadObject<UClass>("/Game/Athena/SupplyDrops/AthenaSupplyDrop_Holiday.AthenaSupplyDrop_Holiday_C");

			Utils::Log("Listening on port 7777");
		}
	}
	
	return UWorld::GetWorld()->NetDriver->ClientConnections.Num() > 0;
}

APawn* Gamemode::SpawnDefaultPawnFor(SDK::AGameMode* a1, SDK::AController* a2, SDK::AActor* a3)
{
	FTransform wow = a3->GetTransform();
	return a1->SpawnDefaultPawnAtTransform(a2, wow);
}

__int64 (*OnAircraftEnteredDropZoneOG)(AFortGameModeAthena*);
__int64 (*OnAircraftExitedDropZoneOG)(__int64, __int64);
__int64 Gamemode::OnAircraftEnteredDropZone(AFortGameModeAthena* a1)
{
	for (size_t i = 0; i < SpawnedBots.size(); i++)
	{
		SpawnedBots[i]->State = EBotState::InBus;
	}

	return OnAircraftEnteredDropZoneOG(a1);
}

__int64 Gamemode::OnAircraftExitedDropZone(__int64 a1, AFortAthenaAircraft* a2)
{
	for (size_t i = 0; i < SpawnedBots.size(); i++)
	{
		SpawnedBots[i]->State = EBotState::Landed;
		SpawnedBots[i]->ForceJump(a2);
	}
	
	if (IsDadBro)
	{
		AFortAthenaMutator_DadBro* Wow = (AFortAthenaMutator_DadBro*)((AFortGameStateAthena*)UWorld::GetWorld()->GameState)->GetMutatorByClass(UWorld::GetWorld()->AuthorityGameMode, AFortAthenaMutator_DadBro::StaticClass());
		Wow->DadBroSpawnLocation.Z = -193.048096f;
		AFortAIPawn* Real = Utils::SpawnActor<AFortAIPawn>(Wow->DadBroSpawnLocation, {}, nullptr, Utils::StaticLoadObject<UClass>("/Game/Athena/DADBRO/DADBRO_Pawn.DADBRO_Pawn_C"), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Wow->DadBroPawn = Real;
		Wow->DadBroCodeState = EDadBroState::Active;
		Wow->OnRep_DadBroPawn();
		Wow->OnRep_DadBroCodeState();
	}

	return OnAircraftExitedDropZoneOG(a1, (__int64)a2);
}

void Gamemode::InitHooks()
{
	static auto Base = __int64(GetModuleHandleW(0));
	Utils::Log("Initializing Gamemode Hooks...");

	MH_CreateHook((LPVOID)(Base + 0x1540390), Gamemode::ReadyToStartMatch, (LPVOID*)&ReadyToStartMatchOG);
	MH_EnableHook((LPVOID)(Base + 0x1540390));
	MH_CreateHook((LPVOID)(Base + 0x154A770), Gamemode::SpawnDefaultPawnFor, nullptr);
	MH_EnableHook((LPVOID)(Base + 0x154A770));
	MH_CreateHook((LPVOID)(Base + 0x15353E0), Gamemode::OnAircraftEnteredDropZone, (LPVOID*)&OnAircraftEnteredDropZoneOG);
	MH_EnableHook((LPVOID)(Base + 0x15353E0));
	MH_CreateHook((LPVOID)(Base + 0x1535480), Gamemode::OnAircraftExitedDropZone, (LPVOID*)&OnAircraftExitedDropZoneOG);
	MH_EnableHook((LPVOID)(Base + 0x1535480));
}