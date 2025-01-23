#include "PC.h"
#include "Quest.h"
#include "minhook/MinHook.h"

#pragma comment(lib, "minhook/minhook.lib")

using namespace SDK;

void (*LoadingScreenDroppedOG)(AFortPlayerControllerAthena*);
void (*GetPlayerViewPointOG)(AFortPlayerControllerAthena* PC, FVector& Loc, FRotator& Rot);
void (*ServerAttemptInteractOG)(UFortControllerComponent_Interaction* Comp, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, ETInteractionType InteractType, UObject* OptionalData, EInteractionBeingAttempted InteractionBeingAttempted);
__int64 (*OnDamageServerOG)(ABuildingSMActor* Actor, float Damage, FGameplayTagContainer DamageTags, FVector Momentum, FHitResult HitInfo, AFortPlayerControllerAthena* InstigatedBy, AActor* DamageCauser, FGameplayEffectContextHandle EffectContext);
void (*ClientOnPawnDiedOG)(AFortPlayerControllerAthena*, FFortPlayerDeathReport);
void (*DestroyPickupOG)(AFortPickup*);
static void (*HandleStartingNewPlayerOG)(AFortGameModeAthena*, APlayerController*);
static void (*EnterAircraftOG)(UObject* PC, AActor* Aircraft);
static void (*ServerAddMapMarkerOG)(UAthenaMarkerComponent* Comp, const FFortClientMarkerRequest& MarkerRequest);
static void (*ServerSetInAircraftOG)(AFortPlayerStateAthena* PlayerState, bool a2);

UClass* LettersClass = nullptr;
UProperty* QuestItem = nullptr;
UProperty* BackendNameProp = nullptr;

void PC::InitHooks()
{
	static auto Base = __int64(GetModuleHandleW(0));
	Utils::Log("Initializing PC Hooks...");

	MH_CreateHook((LPVOID)(Base + 0x1E3D9E0), LoadingScreenDropped, (LPVOID*)&LoadingScreenDroppedOG);
	MH_EnableHook((LPVOID)(Base + 0x1E3D9E0));
	MH_CreateHook((LPVOID)(Base + 0x1E181D0), GetPlayerViewPoint, (LPVOID*)&GetPlayerViewPointOG);
	MH_EnableHook((LPVOID)(Base + 0x1E181D0));
	MH_CreateHook((LPVOID)(Base + 0x184E490), ServerAttemptInteract, (LPVOID*)&ServerAttemptInteractOG);
	MH_EnableHook((LPVOID)(Base + 0x184E490));
	MH_CreateHook((LPVOID)(Base + 0x2176A20), OnDamageServer, (LPVOID*)&OnDamageServerOG);
	MH_EnableHook((LPVOID)(Base + 0x2176A20));
	MH_CreateHook((LPVOID)(Base + 0x2446300), ClientOnPawnDied, (LPVOID*)&ClientOnPawnDiedOG);
	MH_EnableHook((LPVOID)(Base + 0x2446300));
	MH_CreateHook((LPVOID)(Base + 0x1B06660), DestroyPickup, (LPVOID*)&DestroyPickupOG);
	MH_EnableHook((LPVOID)(Base + 0x1B06660));
	MH_CreateHook((LPVOID)(Base + 0x19A0480), HandleStartingNewPlayer, (LPVOID*)&HandleStartingNewPlayerOG);
	MH_EnableHook((LPVOID)(Base + 0x19A0480));

	Utils::SwapVTable(UFortControllerComponent_Aircraft::StaticClass()->DefaultObject, 0x82, ServerAttemptAircraftJump);
	Utils::SwapVTable(UAthenaMarkerComponent::StaticClass()->DefaultObject, 0x7D, ServerRemoveMapMarker);
	Utils::SwapVTable(UAthenaMarkerComponent::StaticClass()->DefaultObject, 0x7F, ServerAddMapMarker);
	Utils::SwapVTable(AFortPlayerStateAthena::StaticClass()->DefaultObject, 0xFC, ServerSetInAircraft, (LPVOID*)&ServerSetInAircraftOG);
	Utils::SwapVTable(UObject::FindObject<UClass>("BlueprintGeneratedClass Athena_PlayerController.Athena_PlayerController_C")->DefaultObject, 0x230,
	                  ServerCreateBuildingActor);
	Utils::SwapVTable(UObject::FindObject<UClass>("BlueprintGeneratedClass Athena_PlayerController.Athena_PlayerController_C")->DefaultObject, 0x10A,
	                  ServerAcknowledgePossession);
	Utils::SwapVTable(UObject::FindObject<UClass>("BlueprintGeneratedClass Athena_PlayerController.Athena_PlayerController_C")->DefaultObject, 0x1C1,
	                  ServerCheat);
	Utils::SwapVTable(UObject::FindObject<UClass>("BlueprintGeneratedClass Athena_PlayerController.Athena_PlayerController_C")->DefaultObject, 0x1C3,
	                  ServerPlayEmoteItem);
	Utils::SwapVTable(UObject::FindObject<UClass>("BlueprintGeneratedClass Athena_PlayerController.Athena_PlayerController_C")->DefaultObject, 0x4DF,
			  ServerPlaySquadQuickChatMessage);
	Utils::SwapVTable(UObject::FindObject<UClass>("BlueprintGeneratedClass Athena_PlayerController.Athena_PlayerController_C")->DefaultObject, 0x209,
	                  ServerExecuteInventoryItem);
	Utils::SwapVTable(UObject::FindObject<UClass>("BlueprintGeneratedClass Athena_PlayerController.Athena_PlayerController_C")->DefaultObject, 0x232,
	                  ServerEditBuildingActor);
	Utils::SwapVTable(UObject::FindObject<UClass>("BlueprintGeneratedClass Athena_PlayerController.Athena_PlayerController_C")->DefaultObject, 0x22C,
					  ServerRepairBuildingActor);
	Utils::SwapVTable(UObject::FindObject<UClass>("BlueprintGeneratedClass Athena_PlayerController.Athena_PlayerController_C")->DefaultObject, 0x235,
	                  ServerEndEditingBuildingActor);
	Utils::SwapVTable(UObject::FindObject<UClass>("BlueprintGeneratedClass Athena_PlayerController.Athena_PlayerController_C")->DefaultObject, 0x237,
	                  ServerBeginEditingBuildingActor);
	Utils::SwapVTable(UObject::FindObject<UClass>("BlueprintGeneratedClass Athena_PlayerController.Athena_PlayerController_C")->DefaultObject, 0x21D,
	                  ServerAttemptInventoryDrop);
	Utils::SwapVTable(UObject::FindObject<UClass>("BlueprintGeneratedClass PlayerPawn_Athena.PlayerPawn_Athena_C")->DefaultObject, 0x1D6,
	                  ServerHandlePickup);
	Utils::SwapVTable(UObject::FindObject<UClass>("BlueprintGeneratedClass PlayerPawn_Athena.PlayerPawn_Athena_C")->DefaultObject, 0x1E1,
					  ServerSendZiplineState);
	Utils::SwapVTable(UObject::FindObject<UClass>("BlueprintGeneratedClass PlayerPawn_Athena.PlayerPawn_Athena_C")->DefaultObject, 0x1BF,
					  ServerReviveFromDBNO);
}

void PC::ServerSetInAircraft(AFortPlayerStateAthena* PlayerState, bool a2)
{
	AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)PlayerState->Owner;

	if (PC && PC->WorldInventory)
	{
		for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
		{
			if (((UFortWorldItemDefinition*)PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition)->bCanBeDropped)
				Inventory::RemoveItem(PC, PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid);
		}
	}

	return ServerSetInAircraftOG(PlayerState, a2);
}

void PC::ServerAddMapMarker(UAthenaMarkerComponent* Comp, const FFortClientMarkerRequest& MarkerRequest)
{
	AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)Comp->GetOwner();
	AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;

	FFortWorldMarkerData MarkerData{};
	FFortWorldMarkerDataCtor(&MarkerData);

	FMarkerID MarkerID{};
	MarkerID.PlayerID = PlayerState->PlayerID;
	MarkerID.InstanceID = MarkerRequest.InstanceID;

	MarkerData.MarkerType = MarkerRequest.MarkerType;
	MarkerData.Owner = PlayerState;
	MarkerData.WorldPosition = MarkerRequest.WorldPosition;
	MarkerData.WorldNormal = MarkerRequest.WorldNormal;
	MarkerData.WorldPositionOffset = MarkerRequest.WorldPositionOffset;
	MarkerData.MarkerID = MarkerID;

	for (size_t i = 0; i < PlayerState->PlayerTeam->TeamMembers.Num(); i++)
	{
		if (PlayerState->PlayerTeam->TeamMembers[i]->IsA(AFortPlayerControllerAthena::StaticClass()) && PlayerState->PlayerTeam->TeamMembers[i] != PC)
		{
			((AFortPlayerControllerAthena*)PlayerState->PlayerTeam->TeamMembers[i])->MarkerComponent->ClientAddMarker(MarkerData);
		}
	}
}

void PC::ServerRemoveMapMarker(UAthenaMarkerComponent* Comp, FMarkerID MarkerID, ECancelMarkerReason CancelReason)
{
	AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)Comp->GetOwner();
	AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;

	for (size_t i = 0; i < PlayerState->PlayerTeam->TeamMembers.Num(); i++)
	{
		AFortPlayerControllerAthena* pc = (AFortPlayerControllerAthena*)PlayerState->PlayerTeam->TeamMembers[i];
		if (pc->IsA(AFortPlayerControllerAthena::StaticClass()) && PlayerState->PlayerTeam->TeamMembers[i] != PC)
		{
			pc->MarkerComponent->ClientCancelMarker(MarkerID);
		}
	}
}

void PC::EnterAircraft(AFortPlayerControllerAthena* PC, AActor* Aircraft)
{
	auto PCV2 = reinterpret_cast<AFortPlayerController*>(((UActorComponent*)PC)->GetOwner());
	
	if (!PCV2)
		return;
	
	EnterAircraftOG(PC, Aircraft);
	
	for (size_t i = 0; i < PCV2->WorldInventory->Inventory.ItemInstances.Num(); i++)
	{
		if (((UFortWorldItemDefinition*)PCV2->WorldInventory->Inventory.ItemInstances[i]->ItemEntry.ItemDefinition)->bCanBeDropped)
		{
			Inventory::RemoveItem(PCV2, PCV2->WorldInventory->Inventory.ItemInstances[i]->ItemEntry.ItemDefinition, PCV2->WorldInventory->Inventory.ItemInstances[i]->ItemEntry.Count);
			break;
		}
	}
}

void PC::HandleStartingNewPlayer(AFortGameModeAthena* GM, APlayerController* NewPlayer)
{
	auto PlayerController = reinterpret_cast<AFortPlayerControllerAthena*>(NewPlayer);
	auto PlayerState = reinterpret_cast<AFortPlayerState*>(PlayerController->PlayerState);
	
	PlayerState->WorldPlayerId = PlayerController->PlayerState->PlayerID;

	static bool First = false;
	if (!First)
	{	
		First = true;
		
		auto GameState = (AFortGameStateAthena*)UWorld::GetWorld()->GameState;
		GameState->DefaultBattleBus = Utils::StaticLoadObject<UAthenaBattleBusItemDefinition>("/Game/Athena/Items/Cosmetics/BattleBuses/BBID_WinterBus.BBID_WinterBus");
		
		for (size_t i = 0; i < GM->BattleBusCosmetics.Num(); i++)
		{
			GM->BattleBusCosmetics[i] = GameState->DefaultBattleBus;
		}
		
		UObject* SnowSetup = Utils::StaticFindObject<UObject>("/Game/Athena/Apollo/Maps/Apollo_POI_Foundations.Apollo_POI_Foundations:PersistentLevel.BP_ApolloSnowSetup_2");
		
		if (SnowSetup)
		{
			UFloatProperty* Prop = Utils::StaticFindObject<UFloatProperty>("/Game/Athena/Apollo/Environments/Blueprints/CalendarEvents/BP_ApolloSnowSetup.BP_ApolloSnowSetup_C:SnowAmount");
			UFloatProperty* Prop2 = Utils::StaticFindObject<UFloatProperty>("/Game/Athena/Apollo/Environments/Blueprints/CalendarEvents/BP_ApolloSnowSetup.BP_ApolloSnowSetup_C:SnowFalling");
			
			if (Prop && Prop2)
			{
				*(float*)(__int64(SnowSetup) + Prop->Offset) = 100.f;
				*(float*)(__int64(SnowSetup) + Prop2->Offset) = 69.f;
				
				UFunction* Func = SnowSetup->Class->GetFunction("BP_ApolloSnowSetup_C", "OnRep_Snow_Amount");
				UFunction* Func2 = SnowSetup->Class->GetFunction("BP_ApolloSnowSetup_C", "OnRep_SnowFalling");
				
				if (Func)
				{
					bool HasAuthority = true;
					SnowSetup->ProcessEvent(Func, &HasAuthority);
					SnowSetup->ProcessEvent(Func2, &HasAuthority);
				}
			}
		}
	}
	
	return HandleStartingNewPlayerOG(GM, NewPlayer);
}
	
void PC::ServerReviveFromDBNO(AFortPlayerPawnAthena* Pawn, AFortPlayerControllerAthena* EventInstigator)
{
	AFortPlayerControllerAthena* DeadPC = (AFortPlayerControllerAthena*)Pawn->Controller;
	AFortPlayerStateAthena* DeadPlayerState = (AFortPlayerStateAthena*)DeadPC->PlayerState;
	static auto Class = Utils::StaticLoadObject<UClass>("/Game/Abilities/NPC/Generic/GAB_AthenaDBNORevive.GAB_AthenaDBNORevive_C");
	static auto Class2 = Utils::StaticLoadObject<UClass>("/Game/Abilities/NPC/Generic/GAB_AthenaDBNO.GAB_AthenaDBNO_C");
	static auto Prop = Utils::StaticLoadObject<UProperty>("/Game/Abilities/NPC/Generic/GAB_AthenaDBNORevive.GAB_AthenaDBNORevive_C.PlayerPawn");
	for (size_t i = 0; i < DeadPlayerState->AbilitySystemComponent->ActivatableAbilities.Items.Num(); i++)
	{
		Utils::Log(DeadPlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].Ability->GetFullName());
		Utils::Log(DeadPlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].Ability->Class->GetFullName());

		if (DeadPlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].Ability->IsA(Class2))
		{
			DeadPlayerState->AbilitySystemComponent->ClientCancelAbility(DeadPlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].Handle, DeadPlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].ActivationInfo);
			DeadPlayerState->AbilitySystemComponent->ClientEndAbility(DeadPlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].Handle, DeadPlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].ActivationInfo);
			DeadPlayerState->AbilitySystemComponent->ServerEndAbility(DeadPlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].Handle, DeadPlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].ActivationInfo, DeadPlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].ActivationInfo.PredictionKeyWhenActivated);
		}

		//if (DeadPlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].Ability->IsA(Class))//this worked for me on s7 for the yellow animation thingy
		//{
		//	*(AActor**)(__int64(DeadPlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].Ability) + Prop->Offset) = Pawn;
		//	FGameplayEventData TestData{};
		//	TestData.EventTag = Pawn->EventReviveTag;
		//	TestData.Instigator = EventInstigator;
		//	TestData.Target = Pawn;
		//	DeadPlayerState->AbilitySystemComponent->ServerTryActivateAbilityWithEventData(DeadPlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].Handle, DeadPlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].InputPressed, DeadPlayerState->AbilitySystemComponent->ActivatableAbilities.Items[i].ActivationInfo.PredictionKeyWhenActivated, TestData);
		//}
	}
	Pawn->bIsDBNO = false;
	Pawn->OnRep_IsDBNO();
	DeadPC->RespawnPlayerAfterDeath(false);
	DeadPC->ClientOnPawnRevived(EventInstigator);
}

void PC::DestroyPickup(AFortPickup* Pickup)
{
	if(!Pickup->PickupLocationData.PickupTarget)
		return DestroyPickupOG(Pickup);
	AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)Pickup->PickupLocationData.PickupTarget->Controller;
	if (!PC || PC->IsA(AFortAthenaAIBotController::StaticClass()))
		return DestroyPickupOG(Pickup);
	FFortItemEntry& PickupEntry = Pickup->PrimaryPickupItemEntry;
	bool Found = false;

	if (PickupEntry.ItemDefinition->IsStackable())
	{
		for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
		{
			auto& Entry = PC->WorldInventory->Inventory.ReplicatedEntries[i];
			if (Entry.ItemDefinition == PickupEntry.ItemDefinition && Entry.Count < Entry.ItemDefinition->MaxStackSize)
			{
				Found = true;
				Entry.Count += PickupEntry.Count;
				Inventory::UpdateEntry(PC, Entry);
				if (Entry.Count > Entry.ItemDefinition->MaxStackSize)
				{
					if (Entry.ItemDefinition->bAllowMultipleStacks)
					{
						Inventory::GiveItem(PC, Entry.ItemDefinition, Entry.Count - Entry.ItemDefinition->MaxStackSize, 0, false);
					}
					else
					{
						Utils::SpawnPickup(PC->Pawn->K2_GetActorLocation(), Entry.ItemDefinition, EFortPickupSourceTypeFlag::Tossed, EFortPickupSpawnSource::Unset, Entry.Count - Entry.ItemDefinition->MaxStackSize, 0, PC->MyFortPawn);
					}
					Entry.Count = Entry.ItemDefinition->MaxStackSize;
					Inventory::UpdateEntry(PC, Entry);
					PC->WorldInventory->Inventory.MarkItemDirty(Entry);
					PC->WorldInventory->bRequiresLocalUpdate = true;
					PC->WorldInventory->HandleInventoryLocalUpdate();
					break;
				}
			}
		}
		if (!Found)
		{
			Inventory::GiveItem(PC, PickupEntry.ItemDefinition, PickupEntry.Count, PickupEntry.LoadedAmmo, false);
		}
	}
	else
	{
		Inventory::GiveItem(PC, PickupEntry.ItemDefinition, PickupEntry.Count, PickupEntry.LoadedAmmo, false);
	}

	return DestroyPickupOG(Pickup);
}

void PC::ServerAttemptInventoryDrop(AFortPlayerControllerAthena* PC, const FGuid& ItemGuid, int32 Count, bool bTrash)
{
	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid == ItemGuid)
		{
			Utils::SpawnPickup(PC->Pawn->K2_GetActorLocation(), PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition, EFortPickupSourceTypeFlag::Other, EFortPickupSpawnSource::Unset, PC->WorldInventory->Inventory.ReplicatedEntries[i].Count, PC->WorldInventory->Inventory.ReplicatedEntries[i].LoadedAmmo, PC->MyFortPawn);
			Inventory::RemoveItem(PC, ItemGuid);
			break;
		}
	}
}

void PC::ServerBeginEditingBuildingActor(AFortPlayerControllerAthena* PC, ABuildingSMActor* BuildingActorToEdit)
{
	if (!PC || !PC->MyFortPawn || !BuildingActorToEdit)
		return;

	AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;
	if (!PlayerState)
		return;

	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(UFortEditToolItemDefinition::StaticClass()))
		{
			PC->ServerExecuteInventoryItem(PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid);
			break;
		}
	}
}

void PC::ServerEndEditingBuildingActor(AFortPlayerControllerAthena* PC, ABuildingSMActor* BuildingActorToEdit)
{
	if (!PC || !PC->MyFortPawn || !BuildingActorToEdit || BuildingActorToEdit->EditingPlayer != (AFortPlayerStateAthena*)PC->PlayerState)
		return;

	BuildingActorToEdit->SetNetDormancy(ENetDormancy::DORM_DormantAll);
	BuildingActorToEdit->EditingPlayer = nullptr;

	AFortWeap_EditingTool* EditTool = (AFortWeap_EditingTool*)PC->MyFortPawn->CurrentWeapon;

	if (!EditTool)
		return;

	EditTool->EditActor = nullptr;
	EditTool->OnRep_EditActor();
}

void PC::ServerEditBuildingActor(AFortPlayerControllerAthena* PC, ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, uint8 RotationIterations, bool bMirrored)
{
	if (!PC || !BuildingActorToEdit || !NewBuildingClass.Get())
		return;

	BuildingActorToEdit->SetNetDormancy(ENetDormancy::DORM_DormantAll);
	BuildingActorToEdit->EditingPlayer = nullptr;

	ABuildingSMActor* NewBuild = ReplaceBuildingActor(BuildingActorToEdit, 1, NewBuildingClass.Get(), 0, RotationIterations, bMirrored, PC);

	if (NewBuild)
		NewBuild->bPlayerPlaced = true;
}

void PC::ServerRepairBuildingActor(AFortPlayerController* PC, ABuildingSMActor* BuildingActorToRepair)
{
	if (!BuildingActorToRepair || BuildingActorToRepair->EditingPlayer || BuildingActorToRepair->bActorIsBeingDestroyed)
		return;

	const float BuildingCost = 10;
	const float RepairCostMultiplier = 0.75;

	const float RepairCost = max(std::floor((BuildingCost * (1 - BuildingActorToRepair->GetHealthPercent())) * RepairCostMultiplier), 1.0f);
	
	BuildingActorToRepair->RepairBuilding(PC, RepairCost);
	Inventory::RemoveItem(PC, UFortKismetLibrary::GetDefaultObj()->K2_GetResourceItemDefinition(BuildingActorToRepair->ResourceType), RepairCost);
}

void PC::ServerHandlePickup(AFortPlayerPawnAthena* Pawn, AFortPickup* Pickup, float InFlyTime, const FVector& InStartDirection, bool bPlayPickupSound)
{
	if (!Pickup || !Pawn || !Pawn->Controller || Pickup->bPickedUp)
		return;

	AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)Pawn->Controller;
	Pickup->PickupLocationData.bPlayPickupSound = bPlayPickupSound;
	Pickup->PickupLocationData.FlyTime = 0.4f;
	Pickup->PickupLocationData.ItemOwner = Pawn;
	Pickup->PickupLocationData.PickupGuid = Pickup->PrimaryPickupItemEntry.ItemGuid;
	Pickup->PickupLocationData.PickupTarget = Pawn;
	Pickup->OnRep_PickupLocationData();

	if (PC->IsA(AFortAthenaAIBotController::StaticClass()))
	{
		for (auto& bot : SpawnedBots)
		{
			if (((void*)bot->PC) == ((void*)PC))
			{
				bot->GiveItem(Pickup->PrimaryPickupItemEntry.ItemDefinition, Pickup->PrimaryPickupItemEntry.Count, Pickup->PrimaryPickupItemEntry.LoadedAmmo);
				if (((UFortWeaponItemDefinition*)Pickup->PrimaryPickupItemEntry.ItemDefinition)->GetAmmoWorldItemDefinition_BP() && ((UFortWeaponItemDefinition*)Pickup->PrimaryPickupItemEntry.ItemDefinition)->GetAmmoWorldItemDefinition_BP() != Pickup->PrimaryPickupItemEntry.ItemDefinition)
				{
					bot->GiveItem(((UFortWeaponItemDefinition*)Pickup->PrimaryPickupItemEntry.ItemDefinition)->GetAmmoWorldItemDefinition_BP(), 9999);
				}
				break;
			}
		}
	}
	//else
	//{
	//	Inventory::GiveItem(PC, Pickup->PrimaryPickupItemEntry.ItemDefinition, Pickup->PrimaryPickupItemEntry.Count, Pickup->PrimaryPickupItemEntry.LoadedAmmo, Pickup->PrimaryPickupItemEntry.ItemDefinition->IsStackable());
	//}

	Pickup->bPickedUp = true;
	Pickup->OnRep_bPickedUp();
}

void PC::ServerSendZiplineState(AFortPlayerPawn* Pawn, FZiplinePawnState InZiplineState)
{
	if (InZiplineState.AuthoritativeValue > Pawn->ZiplineState.AuthoritativeValue)
	{
		Pawn->ZiplineState = InZiplineState;
		OnRep_ZiplineState(Pawn);
		
		if (!Pawn->ZiplineState.bIsZiplining)
		{
			if (Pawn->ZiplineState.bJumped)
			{
				float ZiplineJumpDampening = -0.5f;
				float ZiplineJumpStrength = 1500.f;

				auto CharacterMovement = Pawn->CharacterMovement;
				auto Velocity = CharacterMovement->Velocity;

				FVector LaunchVelocity = {-750, -750, ZiplineJumpStrength};

				if (ZiplineJumpDampening * Velocity.X >= -750.f)
				{
					LaunchVelocity.X = fminf(ZiplineJumpDampening * Velocity.X, 750);
				}
				if (ZiplineJumpDampening * Velocity.Y >= -750.f)
				{
					LaunchVelocity.Y = fminf(ZiplineJumpDampening * Velocity.Y, 750);
				}
				
				Pawn->LaunchCharacter(LaunchVelocity, false, false);
			}
		}
	}
}

void PC::ClientOnPawnDied(AFortPlayerControllerAthena* DeadPC, FFortPlayerDeathReport DeathReport)
{
	if(IsDadBro)
		return ClientOnPawnDiedOG(DeadPC, DeathReport);

	Utils::Log("Pawn died");
	DeadPC->bMarkedAlive = false;

	auto GameState = (AFortGameStateAthena*)UWorld::GetWorld()->GameState;
	auto GameMode = (AFortGameModeAthena*)UWorld::GetWorld()->AuthorityGameMode;
	AFortPlayerStateAthena* DeadState = (AFortPlayerStateAthena*)DeadPC->PlayerState;
	AFortPlayerPawnAthena* KillerPawn = (AFortPlayerPawnAthena*)DeathReport.KillerPawn;
	AFortPlayerStateAthena* KillerState = (AFortPlayerStateAthena*)DeathReport.KillerPlayerState;
	static bool Won = false;

	if (!GameState->IsRespawningAllowed(DeadState))
	{
		if (DeadPC && DeadPC->WorldInventory)
		{
			for (size_t i = 0; i < DeadPC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
			{
				if (((UFortWorldItemDefinition*)DeadPC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition)->bCanBeDropped)
				{
					Utils::SpawnPickup(DeadPC->Pawn->K2_GetActorLocation(), DeadPC->WorldInventory->Inventory.ItemInstances[i]->ItemEntry.ItemDefinition, EFortPickupSourceTypeFlag::Player, EFortPickupSpawnSource::PlayerElimination, DeadPC->WorldInventory->Inventory.ItemInstances[i]->ItemEntry.Count, DeadPC->WorldInventory->Inventory.ItemInstances[i]->ItemEntry.LoadedAmmo, DeadPC->MyFortPawn);
				}
			}
		}
	}

	if (!Won && DeadPC && DeadState)
	{
		if (KillerState && KillerState != DeadState)
		{
			KillerState->KillScore++;

			for (size_t i = 0; i < KillerState->PlayerTeam->TeamMembers.Num(); i++)
			{
				((AFortPlayerStateAthena*)KillerState->PlayerTeam->TeamMembers[i]->PlayerState)->TeamKillScore++;
				((AFortPlayerStateAthena*)KillerState->PlayerTeam->TeamMembers[i]->PlayerState)->OnRep_TeamKillScore();
			}
			Accolades::GiveAccolade((AFortPlayerControllerAthena*)KillerState->Owner, GetDefFromEvent(EAccoladeEvent::Kill, KillerState->KillScore));

			KillerState->ClientReportKill(DeadState);
			KillerState->OnRep_Kills();

			DeadState->PawnDeathLocation = DeadPC->Pawn->K2_GetActorLocation();
			FDeathInfo& DeathInfo = DeadState->DeathInfo;

			DeathInfo.bDBNO = DeadPC->MyFortPawn->bWasDBNOOnDeath;
			DeathInfo.bInitialized = true;
			DeathInfo.DeathLocation = DeadPC->Pawn->K2_GetActorLocation();
			DeathInfo.DeathTags = DeathReport.Tags;
			DeathInfo.Downer = KillerState;
			DeathInfo.Distance = (KillerPawn ? KillerPawn->GetDistanceTo(DeadPC->Pawn) : ((AFortPlayerPawnAthena*)DeadPC->Pawn)->LastFallDistance);
			DeathInfo.FinisherOrDowner = KillerState;
			DeathInfo.DeathCause = DeadState->ToDeathCause(DeathInfo.DeathTags, DeathInfo.bDBNO);
			DeadState->OnRep_DeathInfo();
		}

		if (Won || !GameState->IsRespawningAllowed(DeadState))
		{
			FAthenaRewardResult Result;
			UFortPlayerControllerAthenaXPComponent* XPComponent = DeadPC->XPComponent;
			Result.TotalBookXpGained = XPComponent->TotalXpEarned;
			Result.TotalSeasonXpGained = XPComponent->TotalXpEarned;

			DeadPC->ClientSendEndBattleRoyaleMatchForPlayer(true, Result);

			FAthenaMatchStats Stats;
			FAthenaMatchTeamStats TeamStats;

			if (DeadState)
			{
				DeadState->Place = GameMode->AliveBots.Num() + GameMode->AlivePlayers.Num();
				DeadState->OnRep_Place();
			}

			for (size_t i = 0; i < 20; i++)
			{
				Stats.Stats[i] = 0;
			}
			
			Stats.Stats[3] = DeadState->KillScore;

			TeamStats.Place = DeadState->Place;
			TeamStats.TotalPlayers = GameState->TotalPlayers;

			DeadPC->ClientSendMatchStatsForPlayer(Stats);
			DeadPC->ClientSendTeamStatsForPlayer(TeamStats);
			FDeathInfo& DeathInfo = DeadState->DeathInfo;

			RemoveFromAlivePlayers(GameMode, DeadPC, (KillerState == DeadState ? nullptr : KillerState), KillerPawn, DeathReport.KillerWeapon ? DeathReport.KillerWeapon : nullptr, DeadState ? DeathInfo.DeathCause : EDeathCause::Rifle, 0);

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

			if (KillerState)
			{
				if (KillerState->Place == 1)
				{
					if (DeathReport.KillerWeapon)
					{
						((AFortPlayerControllerAthena*)KillerState->Owner)->PlayWinEffects(KillerPawn, DeathReport.KillerWeapon, EDeathCause::Rifle, false);
						((AFortPlayerControllerAthena*)KillerState->Owner)->ClientNotifyWon(KillerPawn, DeathReport.KillerWeapon, EDeathCause::Rifle);
					}

					FAthenaRewardResult Result;
					AFortPlayerControllerAthena* KillerPC = (AFortPlayerControllerAthena*)KillerState->GetOwner();
					KillerPC->ClientSendEndBattleRoyaleMatchForPlayer(true, Result);

					FAthenaMatchStats Stats;
					FAthenaMatchTeamStats TeamStats;

					for (size_t i = 0; i < 20; i++)
					{
						Stats.Stats[i] = 0;
					}

					Stats.Stats[3] = KillerState->KillScore;
					
					TeamStats.Place = 1;
					TeamStats.TotalPlayers = GameState->TotalPlayers;

					KillerPC->ClientSendMatchStatsForPlayer(Stats);
					KillerPC->ClientSendTeamStatsForPlayer(TeamStats);

					GameState->WinningPlayerState = KillerState;
					GameState->WinningTeam = KillerState->TeamIndex;
					GameState->OnRep_WinningPlayerState();
					GameState->OnRep_WinningTeam();
				}
			}
		}
	}

	return ClientOnPawnDiedOG(DeadPC, DeathReport);
}

void PC::ServerCreateBuildingActor(AFortPlayerControllerAthena* PC, FCreateBuildingActorData CreateBuildingData)
{
	if (!PC || PC->IsInAircraft())
		return;

	UClass* BuildingClass = PC->BroadcastRemoteClientInfo->RemoteBuildableClass.Get();
	char a7;
	TArray<AActor*> BuildingsToRemove;
	if (!CantBuild(UWorld::GetWorld(), BuildingClass, CreateBuildingData.BuildLoc, CreateBuildingData.BuildRot, CreateBuildingData.bMirrored, &BuildingsToRemove, &a7))
	{
		auto ResDef = UFortKismetLibrary::GetDefaultObj()->K2_GetResourceItemDefinition(((ABuildingSMActor*)BuildingClass->DefaultObject)->ResourceType);
		Inventory::RemoveItem(PC, ResDef, 10);

		ABuildingSMActor* NewBuilding = Utils::SpawnActor<ABuildingSMActor>(CreateBuildingData.BuildLoc, CreateBuildingData.BuildRot, PC, BuildingClass);

		NewBuilding->bPlayerPlaced = true;
		NewBuilding->InitializeKismetSpawnedBuildingActor(NewBuilding, PC, true);
		NewBuilding->TeamIndex = ((AFortPlayerStateAthena*)PC->PlayerState)->TeamIndex;
		NewBuilding->Team = EFortTeam(NewBuilding->TeamIndex);

		for (size_t i = 0; i < BuildingsToRemove.Num(); i++)
		{
			BuildingsToRemove[i]->K2_DestroyActor();
		}
		BuildingsToRemove.FreeArray();
	}
}

__int64 PC::OnDamageServer(ABuildingSMActor* Actor, float Damage, FGameplayTagContainer DamageTags, FVector Momentum, FHitResult HitInfo, AFortPlayerControllerAthena* InstigatedBy, AActor* DamageCauser, FGameplayEffectContextHandle EffectContext)
{
	if (!Actor || !InstigatedBy || !InstigatedBy->IsA(AFortPlayerControllerAthena::StaticClass()) || !DamageCauser->IsA(AFortWeapon::StaticClass()) || !((AFortWeapon*)DamageCauser)->WeaponData->IsA(UFortWeaponMeleeItemDefinition::StaticClass()) || Actor->bPlayerPlaced)
		return OnDamageServerOG(Actor, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
	
	auto Def = UFortKismetLibrary::GetDefaultObj()->K2_GetResourceItemDefinition(Actor->ResourceType);
	
	if (Def)
	{
		auto& BuildingResourceAmountOverride = Actor->BuildingResourceAmountOverride;
		auto GameState = (AFortGameStateAthena*)UWorld::GetWorld()->GameState;

		FString CurveTableAssetPath = UKismetStringLibrary::GetDefaultObj()->Conv_NameToString(GameState->CurrentPlaylistInfo.BasePlaylist->ResourceRates.ObjectID.AssetPathName);
		static auto CurveTable = Utils::StaticLoadObject<UCurveTable>(CurveTableAssetPath.ToString()) ? Utils::StaticLoadObject<UCurveTable>(CurveTableAssetPath.ToString()) : Utils::StaticFindObject<UCurveTable>("/Game/Athena/Balance/DataTables/AthenaResourceRates.AthenaResourceRates");
			
		float Average;
		EEvaluateCurveTableResult OutCurveTable{};
                
		reinterpret_cast<UDataTableFunctionLibrary*>(UDataTableFunctionLibrary::StaticClass()->DefaultObject)->EvaluateCurveTableRow(CurveTable, BuildingResourceAmountOverride.RowName, 0.f, &OutCurveTable, &Average,FString());
		float FinalResourceCount = round(Average / (Actor->GetMaxHealth() / Damage));

		if (FinalResourceCount > 0)
		{
			if (Damage == 100.f)
			{
				Accolades::GiveAccolade(InstigatedBy, Utils::StaticLoadObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_066_WeakSpotsInARow.AccoladeId_066_WeakSpotsInARow"));
			}
			InstigatedBy->ClientReportDamagedResourceBuilding(Actor, Actor->ResourceType, FinalResourceCount, false, Damage == 100.f);
			Inventory::GiveItem(InstigatedBy, Def, FinalResourceCount, 0, true);
		}
	}

	return OnDamageServerOG(Actor, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
}

void PC::ServerAttemptInteract(UFortControllerComponent_Interaction* Comp, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, ETInteractionType InteractType, UObject* OptionalData, EInteractionBeingAttempted InteractionBeingAttempted)
{
	static map<AFortPlayerControllerAthena*, int> ChestsSearched{};
	ServerAttemptInteractOG(Comp, ReceivingActor, InteractComponent, InteractType, OptionalData, InteractionBeingAttempted);
	AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)Comp->GetOwner();
	
	if (PC->MyFortPawn && PC->MyFortPawn->IsInVehicle()) //commented until we fix weapon removing
	{
		auto Vehicle = PC->MyFortPawn->GetVehicle();
		Utils::Log(Vehicle->GetName());
		if (Vehicle)
		{
			auto SeatIdx = PC->MyFortPawn->GetVehicleSeatIndex();
			auto WeaponComp = Vehicle->GetSeatWeaponComponent(SeatIdx);
			if (WeaponComp)
			{
				Inventory::GiveItem(PC, WeaponComp->WeaponSeatDefinitions[SeatIdx].VehicleWeapon, 1, 9999);
				for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
				{
					if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition == WeaponComp->WeaponSeatDefinitions[SeatIdx].VehicleWeapon)
					{
						PC->SwappingItemDefinition = PC->MyFortPawn->CurrentWeapon->WeaponData;
						PC->ServerExecuteInventoryItem(PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid);
						break;
					}
				}
			}
		}
	}

	if (ReceivingActor->IsA(LettersClass))
	{
		FName BackendName = *(FName*)(__int64(ReceivingActor) + BackendNameProp->Offset);
		UFortQuestItemDefinition* QuestDef = *(UFortQuestItemDefinition**)(__int64(ReceivingActor) + QuestItem->Offset);
		Quests::ProgressQuest(PC, QuestDef, BackendName);
	}

	if (ReceivingActor->Class->GetName().contains("Tiered_"))
	{
		ChestsSearched[PC]++;
		Accolades::GiveAccolade(PC, GetDefFromEvent(EAccoladeEvent::Search, ChestsSearched[PC], ReceivingActor));
	}
}

void PC::ServerPlayEmoteItem(AFortPlayerControllerAthena* PC, UFortItemDefinition* EmoteAsset, float)
{
	if (EmoteAsset->IsA(UAthenaDanceItemDefinition::StaticClass()))
	{
		if (auto Pawn = PC->MyFortPawn)
		{
			if (auto DanceItemDefinition = reinterpret_cast<UAthenaDanceItemDefinition*>(EmoteAsset))
			{
				Pawn->bMovingEmote = DanceItemDefinition->bMovingEmote;
				Pawn->bMovingEmoteForwardOnly = DanceItemDefinition->bMoveForwardOnly;
				Pawn->EmoteWalkSpeed = DanceItemDefinition->WalkForwardSpeed;
			}
		}
	}
	
	static UClass* EmoteAbilityClass = Utils::StaticLoadObject<UClass>("/Game/Abilities/Emotes/GAB_Emote_Generic.GAB_Emote_Generic_C");
	
	FGameplayAbilitySpec Spec{};
	AbilitySpecCtor(&Spec, reinterpret_cast<UGameplayAbility*>(EmoteAbilityClass->DefaultObject), 1, -1, EmoteAsset);
	GiveAbilityAndActivateOnce(reinterpret_cast<AFortPlayerStateAthena*>(PC->PlayerState)->AbilitySystemComponent, &Spec.Handle, Spec);
}

void PC::ServerPlaySquadQuickChatMessage(AFortPlayerControllerAthena* PlayerController, FAthenaQuickChatActiveEntry ChatEntry, __int64)
{
	auto PlayerState = reinterpret_cast<AFortPlayerStateAthena*>(PlayerController->PlayerState);
	
	switch (ChatEntry.Index)
	{
	case 0:
		PlayerState->ReplicatedTeamMemberState = ETeamMemberState::ChatBubble;
		break;
	case 1:
		PlayerState->ReplicatedTeamMemberState = ETeamMemberState::EnemySpotted;
		break;
	case 2:
		PlayerState->ReplicatedTeamMemberState = ETeamMemberState::NeedMaterials;
		break;
	case 3:
		PlayerState->ReplicatedTeamMemberState = ETeamMemberState::NeedBandages;
		break;
	case 4:
		PlayerState->ReplicatedTeamMemberState = ETeamMemberState::NeedShields;
		break;
	case 5:
		PlayerState->ReplicatedTeamMemberState = ETeamMemberState::NeedAmmoHeavy;
		break;
	case 6:
		PlayerState->ReplicatedTeamMemberState = ETeamMemberState::NeedAmmoLight;
		break;
	case 7:
		PlayerState->ReplicatedTeamMemberState = ETeamMemberState::FIRST_CHAT_MESSAGE;
		break;
	case 8:
		PlayerState->ReplicatedTeamMemberState = ETeamMemberState::NeedAmmoMedium;
		break;
	case 9:
		PlayerState->ReplicatedTeamMemberState = ETeamMemberState::NeedAmmoShells;
		break;
	default:
		break;
	}
	
	PlayerState->OnRep_ReplicatedTeamMemberState();

	static auto EmojiComm = Utils::StaticFindObject<UAthenaEmojiItemDefinition>("/Game/Athena/Items/Cosmetics/Dances/Emoji/Emoji_Comm.Emoji_Comm");
	
	if (EmojiComm)
	{
		PlayerController->ServerPlayEmoteItem(EmojiComm, 0);
	}
}

void PC::ServerAttemptAircraftJump(UFortControllerComponent_Aircraft* Comp, FRotator ROt)
{
	auto PC = (AFortPlayerControllerAthena*)Comp->GetOwner();
	Utils::GetWorld()->AuthorityGameMode->RestartPlayer(PC);
	
	if (PC->MyFortPawn)
		((AFortPlayerPawnAthena*)PC->MyFortPawn)->BeginSkydiving(true);
}

void PC::LoadingScreenDropped(AFortPlayerControllerAthena* PC)
{
	LoadingScreenDroppedOG(PC);
	((UFortKismetLibrary*)UFortKismetLibrary::StaticClass()->DefaultObject)->UpdatePlayerCustomCharacterPartsVisualization((AFortPlayerState*)PC->PlayerState);
	Inventory::InitInventoryForPlayer(PC);
	Abilities::InitAbilitiesForPlayer(PC);
	
	((AFortPlayerStateAthena*)PC->PlayerState)->SquadId = ((AFortPlayerStateAthena*)PC->PlayerState)->TeamIndex - 2;
	((AFortPlayerStateAthena*)PC->PlayerState)->OnRep_TeamIndex(0);
	((AFortPlayerStateAthena*)PC->PlayerState)->OnRep_SquadId();
	((AFortPlayerStateAthena*)PC->PlayerState)->OnRep_PlayerTeam();
	((AFortPlayerStateAthena*)PC->PlayerState)->OnRep_PlayerTeamPrivate();
	
	FGameMemberInfo Wow{ -1,-1,-1 };
	Wow.MemberUniqueId = PC->PlayerState->UniqueId;
	Wow.SquadId = ((AFortPlayerStateAthena*)PC->PlayerState)->SquadId;
	Wow.TeamIndex = ((AFortPlayerStateAthena*)PC->PlayerState)->TeamIndex;
	
	((AFortGameStateAthena*)UWorld::GetWorld()->GameState)->GameMemberInfoArray.Members.Add(Wow);
	((AFortGameStateAthena*)UWorld::GetWorld()->GameState)->GameMemberInfoArray.MarkItemDirty(Wow);

	PC->XPComponent->bRegisteredWithQuestManager = true;
	PC->XPComponent->OnRep_bRegisteredWithQuestManager();

	static bool First = false;
	if (!First)//1:1 fr
	{
		First = true;
		LettersClass = Utils::StaticLoadObject<UClass>("/Game/Athena/Items/QuestInteractables/FortnightLetters/FortniteLettersBPs/Prop_QuestInteractable_Letters_Parent.Prop_QuestInteractable_Letters_Parent_C");
		QuestItem = Utils::StaticLoadObject<UProperty>("/Game/Athena/Items/QuestInteractables/Generic/Prop_QuestInteractable_Parent.Prop_QuestInteractable_Parent_C.QuestItem");
		BackendNameProp = Utils::StaticLoadObject<UProperty>("/Game/Athena/Items/QuestInteractables/Generic/Prop_QuestInteractable_Parent.Prop_QuestInteractable_Parent_C.QuestBackendObjectiveName");
	}
}

void PC::ServerAcknowledgePossession(AFortPlayerControllerAthena* PC, APawn* Pawn)
{
	PC->AcknowledgedPawn = Pawn;
}

void PC::ServerExecuteInventoryItem(AFortPlayerControllerAthena* PC, FGuid Guid)
{
	if (!PC->Pawn || !PC->MyFortPawn || PC->IsInAircraft())
		return;

	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		auto ItemEntry = PC->WorldInventory->Inventory.ReplicatedEntries[i];
		
		if (ItemEntry.ItemGuid == Guid)
		{
			if (ItemEntry.ItemDefinition->IsA(UFortDecoItemDefinition::StaticClass()))
			{
				PC->MyFortPawn->PickUpActor(nullptr, (UFortDecoItemDefinition*)ItemEntry.ItemDefinition);
				PC->MyFortPawn->CurrentWeapon->ItemEntryGuid = Guid;

				if (PC->MyFortPawn->CurrentWeapon->IsA(AFortDecoTool_ContextTrap::StaticClass()))
				{
					reinterpret_cast<AFortDecoTool_ContextTrap*>(PC->MyFortPawn->CurrentWeapon)->ContextTrapItemDefinition = (UFortContextTrapItemDefinition*)ItemEntry.ItemDefinition;
				}
			}
			PC->MyFortPawn->EquipWeaponDefinition((UFortWeaponItemDefinition*)ItemEntry.ItemDefinition, Guid);
			break;
		}
	}
}

void PC::GetPlayerViewPoint(AFortPlayerControllerAthena* PC, FVector& Loc, FRotator& Rot)
{
	if (PC->StateName.ComparisonIndex == SpectatingName.ComparisonIndex)
	{
		Loc = PC->LastSpectatorSyncLocation;
		Rot = PC->LastSpectatorSyncRotation;
	}
	else if (PC->GetViewTarget())
	{
		Loc = PC->GetViewTarget()->K2_GetActorLocation();
		Rot = PC->GetControlRotation();
	}
}

void PC::ServerCheat(AFortPlayerControllerAthena* PC, FString Msg)
{
	string CheatStr = Msg.ToString();

	if (CheatStr == "StartEvent")
	{
		UObject* Test = UObject::FindObject<UObject>("BP_NewYearTimer_C Athena_NYE_Celebration.Athena_NYE_Celebration.PersistentLevel.BP_NewYearTimer_2");
		Test->ProcessEvent(Test->Class->GetFunction("BP_NewYearTimer_C", "startNYE"), nullptr);
	}
	else if (CheatStr == "spawnbot")
	{
		Bots::SpawnBots();
	}
	else if (CheatStr == "startaircraft")
	{
		UKismetSystemLibrary::GetDefaultObj()->ExecuteConsoleCommand(UWorld::GetWorld(), TEXT("startaircraft"), nullptr);
	}
	else if (CheatStr == "pausesafezone")
	{
		UKismetSystemLibrary::GetDefaultObj()->ExecuteConsoleCommand(UWorld::GetWorld(), TEXT("pausesafezone"), nullptr);
	}
	else if (CheatStr.contains("give "))
	{
		string wid = Utils::SplitString(true, "give ", CheatStr);
		Utils::Log(wid);
		UFortWeaponItemDefinition* WeaponDef = UObject::FindObject<UFortWeaponItemDefinition>(wid + "." + wid);
		if (WeaponDef)
		{
			Inventory::GiveItem(PC, WeaponDef, 1, wid.contains("WID_Hook_Gun_Slide") || wid.contains("WID_Athena_HappyGhost") || wid.contains("WID_Hook_Gun_VR_Ore_T03") ? 10 : 0);
			if ((!wid.contains("WID_Hook_Gun_Slide") && !wid.contains("WID_Athena_HappyGhost") && !wid.contains("WID_Hook_Gun_VR_Ore_T03")) && WeaponDef->GetAmmoWorldItemDefinition_BP() && WeaponDef->GetAmmoWorldItemDefinition_BP() != WeaponDef)
			{
				Inventory::GiveItem(PC, WeaponDef->GetAmmoWorldItemDefinition_BP(), 999);
			}
		}
	}
	else if (CheatStr.contains("accolade"))
	{
		Accolades::GiveAccolade(PC, Utils::StaticLoadObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_014_Elimination_Bronze.AccoladeId_014_Elimination_Bronze"));
	}
}
