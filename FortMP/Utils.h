#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "SDK.hpp"
#include <algorithm>

using namespace std;
using namespace SDK;

static __int64 (*CantBuild)(UWorld*, UObject*, FVector, FRotator, char, void*, char*) = decltype(CantBuild)(__int64(GetModuleHandleW(0)) + 0x19EE0E0);
static bool (*InitListen)(void*,void*,SDK::FURL&,bool,SDK::FString&) = decltype(InitListen)(__int64(GetModuleHandleW(0)) + 0x8EFBA0);
static SDK::UNetDriver* (*CreateNetDriver)(void*,void*,SDK::FName) = decltype(CreateNetDriver)(__int64(GetModuleHandleW(0)) + 0x3B21C20);
static void (*SetWorld)(void*,void*) = decltype(SetWorld)(__int64(GetModuleHandleW(0)) + 0x3882F50);
static void(*GiveAbility)(SDK::UAbilitySystemComponent*, SDK::FGameplayAbilitySpecHandle*, SDK::FGameplayAbilitySpec) = decltype(GiveAbility)(__int64(GetModuleHandleW(0)) + 0xB76E70);
static void (*AbilitySpecCtor)(SDK::FGameplayAbilitySpec*, SDK::UGameplayAbility*, int, int, SDK::UObject*) = decltype(AbilitySpecCtor)(__int64(GetModuleHandleW(0)) + 0xB9AF40);
static bool (*InternalTryActivateAbility)(SDK::UAbilitySystemComponent* AbilitySystemComp, SDK::FGameplayAbilitySpecHandle AbilityToActivate, SDK::FPredictionKey InPredictionKey, SDK::UGameplayAbility** OutInstancedAbility, void* OnGameplayAbilityEndedDelegate, const SDK::FGameplayEventData* TriggerEventData) = decltype(InternalTryActivateAbility)(__int64(GetModuleHandleW(0)) + 0xB78580);
static UObject* (*StaticLoadObjectOG)(UClass* Class, UObject* InOuter, const TCHAR* Name, const TCHAR* Filename, uint32_t LoadFlags, UObject* Sandbox, bool bAllowObjectReconciliation, void*) = decltype(StaticLoadObjectOG)(__int64(GetModuleHandleW(0)) + 0x2849140);
static UObject* (*StaticFindObjectOG)(UClass*, UObject* Package, const wchar_t* OrigInName, bool ExactClass) = decltype(StaticFindObjectOG)(__int64(GetModuleHandleW(0)) + 0x2847D60);
static FGameplayAbilitySpecHandle (*GiveAbilityAndActivateOnce)(UAbilitySystemComponent* ASC, FGameplayAbilitySpecHandle*, FGameplayAbilitySpec) = decltype(GiveAbilityAndActivateOnce)(__int64(GetModuleHandleW(0)) + 0xB76F90);
static void(*RemoveFromAlivePlayers)(AFortGameModeAthena*, AFortPlayerControllerAthena*, APlayerState*, AFortPlayerPawn*, UFortWeaponItemDefinition*, EDeathCause, char) = decltype(RemoveFromAlivePlayers)(__int64(GetModuleHandleW(0)) + 0x15413F0);
static ABuildingSMActor* (*ReplaceBuildingActor)(ABuildingSMActor* BuildingSMActor, unsigned int a2, UObject* a3, unsigned int a4, int a5, bool bMirrored, AFortPlayerControllerAthena* PC) = decltype(ReplaceBuildingActor)(__int64(GetModuleHandleW(0)) + 0x1782040);
static void (*OnRep_ZiplineState)(AFortPlayerPawn* Pawn) = decltype(OnRep_ZiplineState)(__int64(GetModuleHandleW(0)) + 0x1DC5980);
static FFortWorldMarkerData* (*FFortWorldMarkerDataCtor)(FFortWorldMarkerData* a1) = decltype(FFortWorldMarkerDataCtor)(__int64(GetModuleHandleW(0)) + 0x128F150);

inline int CurrentPlayersOnTeam = 0;
inline int MaxPlayersOnTeam = 1;
inline uint8 NextIdx = 0;
inline UDataTable* LootTierData = nullptr;
inline UDataTable* LootPackages = nullptr;

inline bool IsDadBro = false;
inline bool IsSTW = false;
inline bool BotsEnabled = true;

inline SDK::FName SpectatingName;
inline AFortAthenaMutator_Bots* BotMutator = nullptr;

inline map<AActor*, AFortAthenaAIBotController*> ChestsForBots{};
inline vector<UAthenaCharacterItemDefinition*> CIDs{};
inline vector<UAthenaPickaxeItemDefinition*> Pickaxes{};
inline vector<UAthenaBackpackItemDefinition*> Backpacks{};
inline vector<UAthenaGliderItemDefinition*> Gliders{};
inline vector<UAthenaSkyDiveContrailItemDefinition*> Contrails{};
inline vector<UAthenaDanceItemDefinition*> Dances{};

namespace Bots
{
	void SpawnBots();
}

namespace Utils
{
	void Log(std::string Str);
	void Log(const char* Str);
	SDK::UFortEngine* GetEngine();
	SDK::UWorld* GetWorld();
	char __fastcall SpawnLoot(ABuildingContainer* Actor);
	char __fastcall SpawnFloorLoot(ABuildingContainer* Actor);
	void SwapVTable(void* base, int Idx, void* Detour, void** OG = nullptr);
	string SplitString(bool SecondString, string delim, string strtosplit);
	void SpawnPickup(FVector Loc, UFortItemDefinition* Def, EFortPickupSourceTypeFlag Flag, EFortPickupSpawnSource SpawnSource, int Count = 1, int LoadedAmmo = 0, AFortPawn* Owner = nullptr);

	template<typename T>
	inline vector<T*> GetAllObjectsOfClass(UClass* Class = T::StaticClass())
	{
		std::vector<T*> Objects{};

		for (int i = 0; i < UObject::GObjects->Num(); ++i)
		{
			UObject* Object = UObject::GObjects->GetByIndex(i);

			if (!Object)
				continue;

			if (Object->GetFullName().contains("Default"))
				continue;

			if (Object->GetFullName().contains("Test"))
				continue;
			
			if (Object->IsA(Class) && !Object->IsDefaultObject())
			{
				Objects.push_back((T*)Object);
			}
		}

		return Objects;
	}

	inline void sinCos(float* ScalarSin, float* ScalarCos, float Value)
	{
		float quotient = (0.31830988618f * 0.5f) * Value;
		if (Value >= 0.0f)
		{
			quotient = (float)((int)(quotient + 0.5f));
		}
		else
		{
			quotient = (float)((int)(quotient - 0.5f));
		}
		float y = Value - (2.0f * 3.1415926535897932f) * quotient;

		float sign;
		if (y > 1.57079632679f)
		{
			y = 3.1415926535897932f - y;
			sign = -1.0f;
		}
		else if (y < -1.57079632679f)
		{
			y = -3.1415926535897932f - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		float y2 = y * y;

		*ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		*ScalarCos = sign * p;
	}

	inline FQuat FRotToQuat(FRotator Rot)
	{
		const float DEG_TO_RAD = 3.1415926535897932f / (180.f);
		const float DIVIDE_BY_2 = DEG_TO_RAD / 2.f;
		float SP, SY, SR;
		float CP, CY, CR;

		sinCos(&SP, &CP, Rot.Pitch * DIVIDE_BY_2);
		sinCos(&SY, &CY, Rot.Yaw * DIVIDE_BY_2);
		sinCos(&SR, &CR, Rot.Roll * DIVIDE_BY_2);

		FQuat RotationQuat;
		RotationQuat.X = CR * SP * SY - SR * CP * CY;
		RotationQuat.Y = -CR * SP * CY - SR * CP * SY;
		RotationQuat.Z = CR * CP * SY - SR * SP * CY;
		RotationQuat.W = CR * CP * CY + SR * SP * SY;

		return RotationQuat;
	}

	template<typename T>
	T* SpawnActor(SDK::FVector Loc, SDK::FRotator Rot = SDK::FRotator(), SDK::AActor* Owner = nullptr, SDK::UClass* Class = T::StaticClass(), ESpawnActorCollisionHandlingMethod Handle = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
	{
		SDK::FTransform Transform{};
		Transform.Scale3D = SDK::FVector{ 1,1,1 };
		Transform.Translation = Loc;
		Transform.Rotation = FRotToQuat(Rot);
		return (T*)SDK::UGameplayStatics::GetDefaultObj()->FinishSpawningActor(SDK::UGameplayStatics::GetDefaultObj()->BeginDeferredActorSpawnFromClass(SDK::UWorld::GetWorld(), Class, Transform, Handle, Owner), Transform);
	}

	template <typename T>
	static T* StaticFindObject(string ObjectName, UClass* ObjectClass = UObject::StaticClass())
	{
		auto OrigInName = wstring(ObjectName.begin(), ObjectName.end()).c_str();

		return (T*)StaticFindObjectOG(ObjectClass, nullptr, OrigInName, false);
	}

	template<typename T>
	T* StaticLoadObject(string name)
	{
		T* Object = StaticFindObject<T>(name);

		if (!Object)
		{
			auto Name = std::wstring(name.begin(), name.end()).c_str();
			Object = (T*)StaticLoadObjectOG(T::StaticClass(), nullptr, Name, nullptr, 0, nullptr, false, nullptr);
		}

		return Object;
	}
}

namespace Inventory
{
	void InitInventoryForPlayer(SDK::AFortPlayerController*);
	void GiveItem(SDK::AFortPlayerController* PC, SDK::UFortItemDefinition* Def, int Count = 1, int LoadedAmmo = 0, bool Stack = false);
	void RemoveItem(SDK::AFortPlayerController* PC, SDK::UFortItemDefinition* Def, int Count = 1);
	void RemoveItem(SDK::AFortPlayerController* PC, FGuid Guid);
	void UpdateEntry(AFortPlayerController* PC, FFortItemEntry& Entry);
}

namespace Abilities
{
	void InitAbilitiesForPlayer(SDK::AFortPlayerController*);
	void InitHooks();
}

namespace Teams
{
	uint8 PickTeam(SDK::AFortGameModeAthena* Gamemode, uint8 PreferredTeam, SDK::AFortPlayerControllerAthena* PC);
}

enum EBotState : uint8
{
	Warmup,
	InBus,
	SkydivingFromBus,
	Landed,
	Looting,
	MovingToZone,
	LookingForPlayers,
	Stuck,
	MAX
};

inline FRotator ToOrientationRotator(FVector Vector)
{
	static UKismetMathLibrary* Math = UKismetMathLibrary::GetDefaultObj();
	FRotator R;

	// Find yaw.
	R.Yaw = Math->Atan2(Vector.Y, Vector.X) * (180.f / 3.1415926535897932f);

	// Find pitch.
	R.Pitch = Math->Atan2(Vector.Z, Math->Sqrt(Vector.X * Vector.X + Vector.Y * Vector.Y)) * (180.f / 3.1415926535897932f);

	// Find roll.
	R.Roll = 0;

	return R;
}

namespace Accolades
{
	void GiveAccolade(AFortPlayerControllerAthena* PC, UFortAccoladeItemDefinition* Def);
}

enum class EAccoladeEvent : uint8
{
	Kill,
	Search,
	MAX
};


inline UFortAccoladeItemDefinition* GetDefFromEvent(EAccoladeEvent Event, int Count, UObject* Object = nullptr)
{
	UFortAccoladeItemDefinition* Def = nullptr;

	switch (Event)
	{
	case EAccoladeEvent::Kill:
		if (Count == 1)
		{
			Def = Utils::StaticLoadObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_014_Elimination_Bronze.AccoladeId_014_Elimination_Bronze");
		}
		else if (Count == 4)
		{
			Def = Utils::StaticLoadObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_015_Elimination_Silver.AccoladeId_015_Elimination_Silver");
		}
		else if (Count == 8)
		{
			Def = Utils::StaticLoadObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_016_Elimination_Gold.AccoladeId_016_Elimination_Gold");
		}
		else
		{
			Def = Utils::StaticLoadObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_012_Elimination.AccoladeId_012_Elimination");
		}
		break;
	case EAccoladeEvent::Search:
		if (!Object || !Object->Class->GetName().contains("Ammo"))
		{
			if (Count == 3)
			{
				Def = Utils::StaticLoadObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_008_SearchChests_Bronze.AccoladeId_008_SearchChests_Bronze");
			}
			else if (Count == 7)
			{
				Def = Utils::StaticLoadObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_009_SearchChests_Silver.AccoladeId_009_SearchChests_Silver");
			}
			else if (Count == 12)
			{
				Def = Utils::StaticLoadObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_010_SearchChests_Gold.AccoladeId_010_SearchChests_Gold");
			}
			else
			{
				Def = Utils::StaticLoadObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_007_SearchChests.AccoladeId_007_SearchChests");
			}
		}
		else
		{
			Def = Utils::StaticLoadObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_011_SearchAmmoBox.AccoladeId_011_SearchAmmoBox");
		}
		break;
	case EAccoladeEvent::MAX:
		break;
	default:
		break;
	}

	return Def;
}

struct Bot
{
public:
	AFortAthenaAIBotController* PC = nullptr;
	AFortPlayerPawnAthena* Pawn = nullptr;
	AFortPlayerStateAthena* PlayerState = nullptr;
	bool Emoting = false;
	bool ThankedBusDriver = false;
	FVector TargetPOI = FVector();
	AActor* TargetActor = nullptr;
	AActor* PreviousActor = nullptr;
	bool TickEnabled = true;
	float FloatValue = 0.f;
	bool JumpedFromBus = false;
	ABuildingActor* StuckActor = nullptr;
	bool Running = false;
	AActor* TargetGoTo = nullptr;

	EBotState State = EBotState::Warmup;
	EBotState PreviousState = EBotState::MAX;

public:
	void GiveItem(UFortItemDefinition* Def, int Count = 1, int LoadedAmmo = 0)
	{
		UFortWorldItem* Item = (UFortWorldItem*)Def->CreateTemporaryItemInstanceBP(Count, 0);
		Item->OwnerInventory = PC->Inventory;
		Item->ItemEntry.LoadedAmmo = LoadedAmmo;
		PC->Inventory->Inventory.ReplicatedEntries.Add(Item->ItemEntry);
		PC->Inventory->Inventory.ItemInstances.Add(Item);
		PC->Inventory->Inventory.MarkItemDirty(Item->ItemEntry);
		PC->Inventory->HandleInventoryLocalUpdate();
	}

	FFortItemEntry* GetEntry(UFortItemDefinition* Def)
	{
		for (size_t i = 0; i < PC->Inventory->Inventory.ReplicatedEntries.Num(); i++)
		{
			if (PC->Inventory->Inventory.ReplicatedEntries[i].ItemDefinition == Def)
				return &PC->Inventory->Inventory.ReplicatedEntries[i];
		}

		return nullptr;
	}

	ABuildingSMActor* FindNearestBuildingSMActor()
	{
		return nullptr;
		static TArray<AActor*> Array;
		static bool First = false;
		if (!First)
		{
			First = true;
			UGameplayStatics::GetDefaultObj()->GetAllActorsOfClass(UWorld::GetWorld(), ABuildingSMActor::StaticClass(), &Array);
		}
		
		AActor* NearestPoi = nullptr;

		for (size_t i = 0; i < Array.Num(); i++)
		{
			if (!NearestPoi || (((ABuildingSMActor*)NearestPoi)->GetHealth() < 1500 && ((ABuildingSMActor*)NearestPoi)->GetHealth() > 1 && Array[i]->GetDistanceTo(Pawn) < NearestPoi->GetDistanceTo(Pawn)))
			{
				NearestPoi = Array[i];
			}
		}

		return (ABuildingSMActor*)NearestPoi;
	}

	Bot(AActor* SpawnLocator)
	{
		if (!BotsEnabled)
			return;

		auto CID = CIDs[UKismetMathLibrary::GetDefaultObj()->RandomIntegerInRange(0, CIDs.size() - 1)];
		auto Backpack = Backpacks[UKismetMathLibrary::GetDefaultObj()->RandomIntegerInRange(0, Backpacks.size() - 1)];
		auto Glider = Gliders[UKismetMathLibrary::GetDefaultObj()->RandomIntegerInRange(0, Gliders.size() - 1)];
		auto Contrail = Contrails[UKismetMathLibrary::GetDefaultObj()->RandomIntegerInRange(0, Contrails.size() - 1)];

		if (!CID || !CID->HeroDefinition || !Backpack || !Glider || !Contrail)
			return;

		static auto BotPawnClas = Utils::StaticLoadObject<UClass>("/Game/Athena/AI/Phoebe/BP_PlayerPawn_Athena_Phoebe.BP_PlayerPawn_Athena_Phoebe_C");
		Pawn = BotMutator->SpawnBot(BotPawnClas, SpawnLocator, SpawnLocator->K2_GetActorLocation(), SpawnLocator->K2_GetActorRotation(), false);
		PC = (AFortAthenaAIBotController*)Pawn->Controller;
		PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;

		auto PickDef = Pickaxes[UKismetMathLibrary::GetDefaultObj()->RandomIntegerInRange(0, Pickaxes.size() - 1)];
		if (!PickDef)
		{
			TickEnabled = false;
			printf("Null!\n");
			return;
		}
		
		if (PickDef && PickDef->WeaponDefinition)
		{
			GiveItem(PickDef->WeaponDefinition);
		}
		
		auto Entry = GetEntry(PickDef->WeaponDefinition);
		Pawn->EquipWeaponDefinition((UFortWeaponItemDefinition*)Entry->ItemDefinition, Entry->ItemGuid);
		
		if (CID->HeroDefinition)
		{
			if (CID->HeroDefinition->Specializations.IsValid())
			{
				for (size_t i = 0; i < CID->HeroDefinition->Specializations.Num(); i++)
				{
					UFortHeroSpecialization* Spec = Utils::StaticLoadObject<UFortHeroSpecialization>(UKismetStringLibrary::GetDefaultObj()->Conv_NameToString(CID->HeroDefinition->Specializations[i].ObjectID.AssetPathName).ToString());
					if (Spec)
					{
						for (size_t j = 0; j < Spec->CharacterParts.Num(); j++)
						{
							UCustomCharacterPart* Part = Utils::StaticLoadObject<UCustomCharacterPart>(UKismetStringLibrary::GetDefaultObj()->Conv_NameToString(Spec->CharacterParts[j].ObjectID.AssetPathName).ToString());
							if (Part)
							{
								PlayerState->CharacterData.Parts[(uintptr_t)Part->CharacterPartType] = Part;
							}
						}
					}
				}
			}
		}
		
		for (size_t j = 0; j < Backpack->CharacterParts.Num(); j++)
		{
			UCustomCharacterPart* Part = Backpack->CharacterParts[j];
			if (Part)
			{
				PlayerState->CharacterData.Parts[(uintptr_t)Part->CharacterPartType] = Part;
			}
		}

		PC->CosmeticLoadoutBC.Glider = Glider;
		PC->CosmeticLoadoutBC.SkyDiveContrail = Contrail;
		Pawn->CosmeticLoadout = PC->CosmeticLoadoutBC;

		UBlackboardData* BB = Utils::StaticLoadObject<UBlackboardData>("/Game/Athena/AI/Phoebe/BehaviorTrees/BB_Phoebe.BB_Phoebe");

		for (size_t i = 0; i < PC->DigestedBotSkillSets.Num(); i++)
		{
			if (PC->DigestedBotSkillSets[i]->IsA(UFortAthenaAIBotAimingDigestedSkillSet::StaticClass()))
			{
				PC->CacheAimingDigestedSkillSet = (UFortAthenaAIBotAimingDigestedSkillSet*)PC->DigestedBotSkillSets[i];
			}

			if (PC->DigestedBotSkillSets[i]->IsA(UFortAthenaAIBotHarvestDigestedSkillSet::StaticClass()))
			{
				PC->CacheHarvestDigestedSkillSet = (UFortAthenaAIBotHarvestDigestedSkillSet*)PC->DigestedBotSkillSets[i];
			}
			
			if (PC->DigestedBotSkillSets[i]->IsA(UFortAthenaAIBotInventoryDigestedSkillSet::StaticClass()))
			{
				PC->CacheInventoryDigestedSkillSet = (UFortAthenaAIBotInventoryDigestedSkillSet*)PC->DigestedBotSkillSets[i];
			}

			if (PC->DigestedBotSkillSets[i]->IsA(UFortAthenaAIBotLootingDigestedSkillSet::StaticClass()))
			{
				PC->CacheLootingSkillSet = (UFortAthenaAIBotLootingDigestedSkillSet*)PC->DigestedBotSkillSets[i];
			}

			if (PC->DigestedBotSkillSets[i]->IsA(UFortAthenaAIBotMovementDigestedSkillSet::StaticClass()))
			{
				PC->CacheMovementSkillSet = (UFortAthenaAIBotMovementDigestedSkillSet*)PC->DigestedBotSkillSets[i];
			}

			if (PC->DigestedBotSkillSets[i]->IsA(UFortAthenaAIBotPerceptionDigestedSkillSet::StaticClass()))
			{
				PC->CachePerceptionDigestedSkillSet = (UFortAthenaAIBotPerceptionDigestedSkillSet*)PC->DigestedBotSkillSets[i];
			}

			if (PC->DigestedBotSkillSets[i]->IsA(UFortAthenaAIBotPlayStyleDigestedSkillSet::StaticClass()))
			{
				PC->CachePlayStyleSkillSet = (UFortAthenaAIBotPlayStyleDigestedSkillSet*)PC->DigestedBotSkillSets[i];
			}
		}

		//PC->Blackboard = BB;
		//static UProperty* Blackboard1Prop = Utils::StaticLoadObject<UProperty>("/Game/Athena/AI/Phoebe/BP_PhoebePlayerController.BP_PhoebePlayerController_C.Blackboard1");
		//UBlackboardComponent** Comp = (UBlackboardComponent**)(__int64(PC) + Blackboard1Prop->Offset);

		//PC->UseBlackboard(BB, &PC->Blackboard);
		//PC->OnUsingBlackBoard(PC->Blackboard, BB);

		//PC->UseBlackboard(BB, &(*Comp));
		//PC->OnUsingBlackBoard((*Comp), BB);

		//UBehaviorTree* Tree = Utils::StaticLoadObject<UBehaviorTree>("/Game/Athena/AI/Phoebe/BehaviorTrees/BT_Phoebe_Warmup.BT_Phoebe_Warmup"); //so uhg some proper ai phoebe code is still in the game and some evaluators arent stripped maybe so ill look into it later
		//PC->BehaviorTree = Tree;
		//PC->RunBehaviorTree(Tree);
		//PC->BlueprintOnBehaviorTreeStarted();

		PC->PathFollowingComponent->MyNavData = ((UAthenaNavSystem*)UWorld::GetWorld()->NavigationSystem)->MainNavData;
		PC->PathFollowingComponent->OnNavDataRegistered(((UAthenaNavSystem*)UWorld::GetWorld()->NavigationSystem)->MainNavData);

		PlayerState->OnRep_CharacterData();

		Pawn->CapsuleComponent->SetGenerateOverlapEvents(true);
		Pawn->CharacterMovement->bCanWalkOffLedges = true;

		//Pawn->CharacterMovement->bUseControllerDesiredRotation = true;
		//Pawn->CharacterMovement->bOrientRotationToMovement = false;
		//Pawn->bUseControllerRotationYaw = false;
		//Pawn->bUseControllerRotationPitch = false;
		//Pawn->bUseControllerRotationRoll = false;

		TargetActor = FindNearestBuildingSMActor();
		
		TickEnabled = true;
	}

public:
	ABuildingActor* FindNearestChest()
	{
		static auto ChestClass = Utils::StaticLoadObject<UClass>("/Game/Building/ActorBlueprints/Containers/Tiered_Chest_Athena.Tiered_Chest_Athena_C");
		TArray<AActor*> Array;
		UGameplayStatics::GetDefaultObj()->GetAllActorsOfClass(UWorld::GetWorld(), ChestClass, &Array);
		
		AActor* NearestPoi = nullptr;

		for (size_t i = 0; i < Array.Num(); i++)
		{
			AActor* Actor = Array[i];
			if (ChestsForBots.contains(Array[i]) && ChestsForBots[Actor] != PC)
				continue;
			if (!NearestPoi || Array[i]->GetDistanceTo(Pawn) < NearestPoi->GetDistanceTo(Pawn))
			{
				NearestPoi = Array[i];
			}
		}
		Array.FreeArray();
		return (ABuildingActor*)NearestPoi;
	}

	AFortPickupAthena* FindNearestPickup()
	{
		static auto ChestClass = AFortPickupAthena::StaticClass();
		TArray<AActor*> Array;
		UGameplayStatics::GetDefaultObj()->GetAllActorsOfClass(UWorld::GetWorld(), ChestClass, &Array);
		AActor* NearestPoi = nullptr;

		for (size_t i = 0; i < Array.Num(); i++)
		{
			if (!NearestPoi || Array[i]->GetDistanceTo(Pawn) < NearestPoi->GetDistanceTo(Pawn))
			{
				NearestPoi = Array[i];
			}
		}
		Array.FreeArray();
		return (AFortPickupAthena*)NearestPoi;
	}

	AFortPlayerPawnAthena* FindNearestPlayer()
	{
		auto GameMode = (AFortGameModeAthena*)UWorld::GetWorld()->AuthorityGameMode;

		AActor* NearestPoi = nullptr;

		for (size_t i = 0; i < GameMode->AlivePlayers.Num(); i++)
		{
			if (!NearestPoi || (GameMode->AlivePlayers[i]->Pawn && GameMode->AlivePlayers[i]->Pawn->GetDistanceTo(Pawn) < NearestPoi->GetDistanceTo(Pawn)))
			{
				NearestPoi = GameMode->AlivePlayers[i]->Pawn;
			}
		}

		for (size_t i = 0; i < GameMode->AliveBots.Num(); i++)
		{
			if (GameMode->AliveBots[i]->Pawn != Pawn)
			{
				if (!NearestPoi || (GameMode->AliveBots[i]->Pawn && GameMode->AliveBots[i]->Pawn->GetDistanceTo(Pawn) < NearestPoi->GetDistanceTo(Pawn)))
				{
					NearestPoi = GameMode->AliveBots[i]->Pawn;
				}
			}
		}

		return (AFortPlayerPawnAthena*)NearestPoi;
	}

	void GoTo(FVector Loc, float Radius = 0)
	{
		PC->MoveToLocation(Loc, 0, true, false, false, true, nullptr, true);
	}

	void GoTo(AActor* Loc, float Radius = 0)
	{
		if (TargetGoTo == Loc)
			return;

		TargetGoTo = Loc;
		PC->MoveToActor(Loc, 0, true, false, true, nullptr, true);
		Run();
	}
	
	void WalkForward()
	{
		Pawn->AddMovementInput(Pawn->GetActorForwardVector(), 1.f, true);
	}

	void WalkBackwards()
	{
		Pawn->AddMovementInput(UKismetMathLibrary::GetDefaultObj()->NegateVector(Pawn->GetActorForwardVector()), 1.f, true);
	}

	void WalkLeft()
	{
		Pawn->AddMovementInput(UKismetMathLibrary::GetDefaultObj()->NegateVector(Pawn->GetActorRightVector()), 1.f, true);
	}

	void WalkRight()
	{
		Pawn->AddMovementInput(Pawn->GetActorRightVector(), 1.f, true);
	}

	int GetAimMaxOffset()
	{
		return 125;
	}

	FVector GetAimDirection(AActor* Actor)
	{
		int MaxOffset = GetAimMaxOffset();
		FVector Loc = Actor->K2_GetActorLocation();
		Loc.X += UKismetMathLibrary::GetDefaultObj()->RandomIntegerInRange(-MaxOffset, MaxOffset);
		Loc.Y += UKismetMathLibrary::GetDefaultObj()->RandomIntegerInRange(-MaxOffset, MaxOffset);
		Loc.Z += UKismetMathLibrary::GetDefaultObj()->RandomIntegerInRange(-MaxOffset, MaxOffset);
		return Loc;
	}

	void LookAt(AActor* Actor, bool Bloom = false)
	{
		if (!Actor || !Pawn || PC->GetFocusActor() == Actor)
			return;

		if (!Actor)
		{
			PC->K2_ClearFocus();
			return;
		}

		if (Bloom)
		{
			PC->K2_SetFocalPoint(GetAimDirection(Actor));
		}
		else
		{
			PC->K2_SetFocus(Actor);
		}
	}
	
	void Skydive(bool FromBus = true)
	{
		Pawn->BeginSkydiving(FromBus);
	}

	void ForceJump(AActor* a2)
	{
		if (!TickEnabled || !Pawn || JumpedFromBus)
			return;
		
		Pawn->K2_TeleportTo(a2->K2_GetActorLocation(), {});
		Skydive();
		
		TargetPOI = FindNearestChest()->K2_GetActorLocation();
		State = EBotState::Landed;
	}

	void EquipGun()
	{
		if (!Pawn || !Pawn->CurrentWeapon)
			return;

		if (Pawn->CurrentWeapon->WeaponData->IsA(UFortWeaponMeleeItemDefinition::StaticClass()))
		{
			for (size_t i = 0; i < PC->Inventory->Inventory.ReplicatedEntries.Num(); i++)
			{
				if (!PC->Inventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(UFortWeaponMeleeItemDefinition::StaticClass()))
				{
					Pawn->EquipWeaponDefinition((UFortWeaponItemDefinition*)PC->Inventory->Inventory.ReplicatedEntries[i].ItemDefinition, PC->Inventory->Inventory.ReplicatedEntries[i].ItemGuid);
					break;
				}
			}
		}
	}

	void EquipPickaxe()
	{
		if (!Pawn || !Pawn->CurrentWeapon)
			return;
		
		if (!Pawn->CurrentWeapon->WeaponData->IsA(UFortWeaponMeleeItemDefinition::StaticClass()))
		{
			for (size_t i = 0; i < PC->Inventory->Inventory.ReplicatedEntries.Num(); i++)
			{
				if (PC->Inventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(UFortWeaponMeleeItemDefinition::StaticClass()))
				{
					Pawn->EquipWeaponDefinition((UFortWeaponItemDefinition*)PC->Inventory->Inventory.ReplicatedEntries[i].ItemDefinition, PC->Inventory->Inventory.ReplicatedEntries[i].ItemGuid);
					break;
				}
			}
		}
	}

	bool HasGun()
	{
		for (size_t i = 0; i < PC->Inventory->Inventory.ReplicatedEntries.Num(); i++)
		{
			if (!PC->Inventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(UFortWeaponMeleeItemDefinition::StaticClass()))
				return true;
		}
		return false;
	}

	void StartLooting()
	{
		TargetActor = FindNearestChest();
		if (TargetActor)
		{
			ChestsForBots[TargetActor] = PC;
			LookAt(TargetActor);
			State = EBotState::Looting;
		}
	}
	
	void Run()
	{
		if (!Running)
		{
			Running = true;
			for (size_t i = 0; i < PlayerState->AbilitySystemComponent->ActivatableAbilities.Items.Num(); i++)
			{
				if (PlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].Ability->IsA(UFortGameplayAbility_Sprint::StaticClass()))
				{
					PlayerState->AbilitySystemComponent->ServerTryActivateAbility(PlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].Handle, PlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].InputPressed, PlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].ActivationInfo.PredictionKeyWhenActivated);
					break;
				}
			}
		}
	}

	void Emote()
	{
		if (Emoting)
			return;
		auto EmoteDef = Dances[UKismetMathLibrary::GetDefaultObj()->RandomIntegerInRange(0, Dances.size() - 1)];
		if (!EmoteDef)
			return;
		Emoting = true;

		static UClass* EmoteAbilityClass = Utils::StaticLoadObject<UClass>("/Game/Abilities/Emotes/GAB_Emote_Generic.GAB_Emote_Generic_C");

		FGameplayAbilitySpec Spec{};
		AbilitySpecCtor(&Spec, reinterpret_cast<UGameplayAbility*>(EmoteAbilityClass->DefaultObject), 1, -1, EmoteDef);
		GiveAbilityAndActivateOnce(reinterpret_cast<AFortPlayerStateAthena*>(PC->PlayerState)->AbilitySystemComponent, &Spec.Handle, Spec);
	}

	virtual void Tick()
	{
		if (!TickEnabled || !Pawn)
			return;

		auto GameState = (AFortGameStateAthena*)UWorld::GetWorld()->GameState;
		
		switch (State)
		{
		case Warmup:
			WalkForward();
			Run();
			if (UKismetMathLibrary::GetDefaultObj()->RandomBoolWithWeight(0.04f))
				Pawn->Jump();
			break;
		case InBus:
			if (!ThankedBusDriver && UKismetMathLibrary::GetDefaultObj()->RandomBoolWithWeight(0.05f))
			{
				ThankedBusDriver = true;
				PC->ThankBusDriver();
			}
			if (UKismetMathLibrary::GetDefaultObj()->RandomBoolWithWeight(0.07f))
			{
				JumpedFromBus == true;
				Pawn->K2_TeleportTo(GameState->GetAircraft(0)->K2_GetActorLocation(), {});
				Skydive(true);
				State = EBotState::Landed;
			}
			break;
		case SkydivingFromBus://kinda useless
			break;
		case Landed:
			StartLooting();
			break;
		case Looting:
			if (TargetActor)
			{
				if (Pawn->IsSkydiving())
				{
					Pawn->AddMovementInput(UKismetMathLibrary::GetDefaultObj()->GetDirectionUnitVector(Pawn->K2_GetActorLocation(), TargetActor->K2_GetActorLocation()), 1, true);
					Pawn->AddMovementInput(UKismetMathLibrary::GetDefaultObj()->NegateVector(Pawn->GetActorUpVector()), 1, true);
					Running = false;
				}
				else
				{
					GoTo(TargetActor);
					if (Pawn->GetDistanceTo(TargetActor) <= 200)
					{
						Pawn->PawnStopFire(0);
						if (!FloatValue)
						{
							FloatValue = UGameplayStatics::GetDefaultObj()->GetTimeSeconds(UWorld::GetWorld());
							Pawn->bStartedInteractSearch = true;
							Pawn->OnRep_StartedInteractSearch();
						}
						else if (UGameplayStatics::GetDefaultObj()->GetTimeSeconds(UWorld::GetWorld()) - FloatValue >= 1.5f)
						{
							Utils::SpawnLoot((ABuildingContainer*)TargetActor);
							AFortPickup* Pickup = FindNearestPickup();
							if (Pickup)
							{
								Pawn->ServerHandlePickup(Pickup, .4f, {}, true);
								if (HasGun())
								{
									EquipGun();
									State = EBotState::LookingForPlayers;
								}
							}
							else
							{
								FloatValue = 0;
								StartLooting();
								Utils::Log("Invalid Pickup");
							}
						}
					}
					else
					{
						Pawn->PawnStartFire(0);
					}
				}
			}
			else
			{
				Utils::Log("Invalid TargetActor, not enough chests????");
				StartLooting();
			}
			break;
		case MovingToZone:
			TargetActor = HasGun() ? FindNearestPlayer() : nullptr;
			if (TargetActor && Pawn->GetDistanceTo(TargetActor) < 4000)
				State = EBotState::LookingForPlayers;

			if (GameState && GameState->SafeZoneIndicator)
			{
				GoTo(GameState->SafeZoneIndicator->NextCenter, GameState->SafeZoneIndicator->NextRadius);
			}
			else
			{
				StartLooting();
			}

			break;
		case LookingForPlayers:
			if (Emoting && UGameplayStatics::GetDefaultObj()->GetTimeSeconds(UWorld::GetWorld()) - FloatValue < 3.f)
			{
				return;
			}
			else if (Emoting)
			{
				FloatValue = 0.f;
				Emoting = false;
			}
			TargetActor = HasGun() ? FindNearestPlayer() : nullptr;
			if (TargetActor && Pawn->GetDistanceTo(TargetActor) < 4000)
			{
				LookAt(TargetActor, true);
				GoTo(TargetActor);
				if (Pawn->GetDistanceTo(TargetActor) > 1500)
				{
					Pawn->Crouch(false);
					Pawn->PawnStopFire(0);
				}
				else if(PC->LineOfSightTo(TargetActor, {}, true))
				{
					Pawn->UnCrouch(false);
					if (UKismetMathLibrary::GetDefaultObj()->RandomBoolWithWeight(0.45f))
					{
						Pawn->PawnStartFire(0);
					}
					else
					{
						Pawn->PawnStopFire(0);
					}
					EquipGun();

					AFortPlayerPawnAthena* Target = (AFortPlayerPawnAthena*)TargetActor;
					if (UKismetMathLibrary::GetDefaultObj()->RandomBoolWithWeight(0.6f) && (!Target || Target->IsDead() || Target->GetHealth() <= 1))
					{
						PC->StopMovement();
						Pawn->PawnStopFire(0);
						Emote();
						FloatValue = UGameplayStatics::GetDefaultObj()->GetTimeSeconds(UWorld::GetWorld());
					}
				}
			}
			else
			{
				State = EBotState::MovingToZone;
			}
			break;
		case Stuck:
		{
			PC->StopMovement();
			Pawn->Crouch(false);
			EquipPickaxe();
			LookAt(StuckActor);
			Pawn->PawnStartFire(0);
			//float Health = StuckActor->GetHealth();
			if (!StuckActor || StuckActor->GetHealth() <= 1)
			{
				StuckActor = nullptr;
				State = PreviousState;
				cout << "Previous State" << (uintptr_t)PreviousState << endl;
				cout << TargetActor << endl;
				LookAt(TargetActor);
				GoTo(TargetActor);
				Pawn->UnCrouch(false);
				Pawn->PawnStopFire(0);
			}
			break;
		}
		case MAX:
			break;
		default:
			break;
		}
	}

	virtual void OnActorBump(AActor* OtherActor, FHitResult& Hit)
	{
		if (State == EBotState::Stuck || StuckActor == OtherActor)
			return;
		if (OtherActor->IsA(ABuildingSMActor::StaticClass()))
		{
			ABuildingSMActor* Actor = (ABuildingSMActor*)OtherActor;
			float Health = Actor->GetHealth();
			FFindFloorResult Res;
			
			if (Actor->bCanBeDamaged == 1 && Health > 1 && Health < 2500)
			{
				Pawn->CharacterMovement->K2_FindFloor(Pawn->CapsuleComponent->K2_GetComponentLocation(), &Res);
				if (Res.HitResult.Actor.Get() == OtherActor)
					return;
				StuckActor = Actor;
				PreviousState = State;
				State = EBotState::Stuck;
			}
		}
	}

	virtual void OnSafeZoneStateChange()
	{
		auto State = ((AFortGameStateAthena*)UWorld::GetWorld()->GameState)->GamePhaseStep;
		
		cout << (uintptr_t)State << endl;
	}

	virtual void OnDied(AFortPlayerStateAthena* KillerState)
	{
		if (!KillerState)
			return;

		Utils::Log("Bot Died");

		TickEnabled = false;
		FDeathInfo& DeathInfo = PlayerState->DeathInfo;
		
		DeathInfo.bDBNO = Pawn->bWasDBNOOnDeath;
		DeathInfo.bInitialized = true;
		DeathInfo.DeathLocation = Pawn->K2_GetActorLocation();
		//DeathInfo.DeathTags = 
		DeathInfo.Downer = KillerState;
		DeathInfo.Distance = (KillerState->GetCurrentPawn() ? KillerState->GetCurrentPawn()->GetDistanceTo(Pawn) : ((AFortPlayerPawnAthena*)Pawn)->LastFallDistance);
		DeathInfo.FinisherOrDowner = KillerState;
		DeathInfo.DeathCause = PlayerState->ToDeathCause(DeathInfo.DeathTags, DeathInfo.bDBNO);
		PlayerState->OnRep_DeathInfo();

		if (!PC->Inventory)
			return;
		
		for (size_t i = 0; i < PC->Inventory->Inventory.ReplicatedEntries.Num(); i++)
		{
			if (PC->Inventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(UFortWeaponMeleeItemDefinition::StaticClass()))
				continue;
			Utils::SpawnPickup(Pawn->K2_GetActorLocation(), PC->Inventory->Inventory.ReplicatedEntries[i].ItemDefinition, EFortPickupSourceTypeFlag::AI, EFortPickupSpawnSource::PlayerElimination, PC->Inventory->Inventory.ReplicatedEntries[i].Count, PC->Inventory->Inventory.ReplicatedEntries[i].LoadedAmmo);
		}

		auto KillerPC = (AFortPlayerControllerAthena*)KillerState->GetOwner();
		if (KillerPC && KillerPC->IsA(AFortPlayerControllerAthena::StaticClass()))
		{
			KillerState->KillScore++;
			Accolades::GiveAccolade(KillerPC, GetDefFromEvent(EAccoladeEvent::Kill, KillerState->KillScore));

			for (size_t i = 0; i < KillerState->PlayerTeam->TeamMembers.Num(); i++)
			{
				((AFortPlayerStateAthena*)KillerState->PlayerTeam->TeamMembers[i]->PlayerState)->TeamKillScore++;
				((AFortPlayerStateAthena*)KillerState->PlayerTeam->TeamMembers[i]->PlayerState)->OnRep_TeamKillScore();
			}

			KillerState->ClientReportKill(PlayerState);
			KillerState->OnRep_Kills();
		}

		AFortGameModeAthena* GameMode = (AFortGameModeAthena*)UWorld::GetWorld()->AuthorityGameMode;
		if (GameMode->AlivePlayers.Num() + GameMode->AliveBots.Num() == 50)
		{
			for (size_t i = 0; i < GameMode->AlivePlayers.Num(); i++)
			{
				Accolades::GiveAccolade(GameMode->AlivePlayers[i], Utils::StaticLoadObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_026_Survival_Default_Bronze.AccoladeId_026_Survival_Default_Bronze"));
			}
		}
		if (GameMode->AlivePlayers.Num() + GameMode->AliveBots.Num() == 25)
		{
			for (size_t i = 0; i < GameMode->AlivePlayers.Num(); i++)
			{
				Accolades::GiveAccolade(GameMode->AlivePlayers[i], Utils::StaticLoadObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_027_Survival_Default_Silver.AccoladeId_027_Survival_Default_Silver"));
			}
		}
		if (GameMode->AlivePlayers.Num() + GameMode->AliveBots.Num() == 10)
		{
			for (size_t i = 0; i < GameMode->AlivePlayers.Num(); i++)
			{
				Accolades::GiveAccolade(GameMode->AlivePlayers[i], Utils::StaticLoadObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_028_Survival_Default_Gold.AccoladeId_028_Survival_Default_Gold"));
			}
		}
	}
};

inline vector<Bot*> SpawnedBots{};