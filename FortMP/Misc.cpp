#include "Misc.h"
#include "Utils.h"
#include <intrin.h>
#include "Quest.h"
#include "minhook/MinHook.h"

#pragma comment(lib, "minhook/minhook.lib")

__int64 (*DispatchRequestOG)(__int64 a1, unsigned __int64* a2, int a3);
__int64 (*PEOG)(void*,void*,void*);
__int64 (*ServerOnExitVehicleOG)(AFortPlayerPawn* Pawn, unsigned __int8);
void (*OnCapsuleBeginOverlapOG)(UObject* a1, UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, FHitResult SweepResult);
void (*OnPossessedPawnDiedOG)(AAIController* PC, AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector Momentum);
char (*InventoryOG)(AActor* a1, __int64 a2);
void (*StormOG)(__int64, int);
void (*MovingEmoteStoppedOG)(AFortPawn* Pawn, void*, void*);
void (*ABuildingSMActor_PostUpdateOG)(ABuildingSMActor*);
char (*GetSquadIdForCurrentPlayerOG)(AFortGameSessionDedicatedAthena* GameSession, FUniqueNetIdRepl UniqueId);
void (*NetMulticast_Athena_BatchedDamageCuesOG)(AFortPawn* Pawn, void* a2, void* a3);
void (*OnReloadOG)(AFortWeapon* a1, unsigned int a2);
void (*UpdateControlRotationOG)(AAIController* PC, float DeltaTime, bool bUpdatePawn);
void (*OnActorBumpOG)(UPathFollowingComponent* Comp, AActor* SelfActor, AActor* OtherActor, const FVector& NormalImpulse, FHitResult& Hit);
void (*GatherNearestLootOG)(__int64 Eval);

void Misc::InitHooks()
{
	static auto Base = __int64(GetModuleHandleW(0));
	Utils::Log("Initializing Misc Hooks...");

	MH_CreateHook((LPVOID)(Base + 0x27C47A0), Misc::CollectGarbageInternal, nullptr);
	MH_EnableHook((LPVOID)(Base + 0x27C47A0));
	MH_CreateHook((LPVOID)(Base + 0x27E09E0), Misc::CollectGarbageInternal, nullptr);
	MH_EnableHook((LPVOID)(Base + 0x27E09E0));
	MH_CreateHook((LPVOID)(Base + 0x27E8B00), Misc::Context, nullptr);
	MH_EnableHook((LPVOID)(Base + 0x27E8B00));
	MH_CreateHook((LPVOID)(Base + 0xE2BF70), Misc::DispatchRequest, (LPVOID*)&DispatchRequestOG);
	MH_EnableHook((LPVOID)(Base + 0xE2BF70));
	MH_CreateHook((LPVOID)(Base + 0x1DCF3E0), Misc::OnCapsuleBeginOverlap, (LPVOID*)&OnCapsuleBeginOverlapOG);
	MH_EnableHook((LPVOID)(Base + 0x1DCF3E0));
	MH_CreateHook((LPVOID)(Base + 0x1DE3510), Misc::ServerOnExitVehicle, (LPVOID*)&ServerOnExitVehicleOG);
	MH_EnableHook((LPVOID)(Base + 0x1DE3510));
	MH_CreateHook((LPVOID)(Base + 0x12A7810), Misc::Inventory, (LPVOID*)&InventoryOG);
	MH_EnableHook((LPVOID)(Base + 0x12A7810));
	MH_CreateHook((LPVOID)(Base + 0x12AB760), Misc::OnPossessedPawnDied, (LPVOID*)&OnPossessedPawnDiedOG);
	MH_EnableHook((LPVOID)(Base + 0x12AB760));
	MH_CreateHook((LPVOID)(Base + 0x1551340), Misc::Storm, (LPVOID*)&StormOG);
	MH_EnableHook((LPVOID)(Base + 0x1551340));
	MH_CreateHook((LPVOID)(Base + 0xBE8270), Misc::MovingEmoteStopped, (LPVOID*)&MovingEmoteStoppedOG);
	MH_EnableHook((LPVOID)(Base + 0xBE8270));
	MH_CreateHook((LPVOID)(Base + 0x1780130), Misc::ABuildingSMActor_PostUpdate, (LPVOID*)&ABuildingSMActor_PostUpdateOG);
	MH_EnableHook((LPVOID)(Base + 0x1780130));
	MH_CreateHook((LPVOID)(Base + 0x1C1F540), Misc::GetSquadIdForCurrentPlayer, (LPVOID*)&GetSquadIdForCurrentPlayerOG);
	MH_EnableHook((LPVOID)(Base + 0x1C1F540));
	MH_CreateHook((LPVOID)(Base + 0x23F7310), Misc::NetMulticast_Athena_BatchedDamageCues, (LPVOID*)&NetMulticast_Athena_BatchedDamageCuesOG);
	MH_EnableHook((LPVOID)(Base + 0x23F7310));
	MH_CreateHook((LPVOID)(Base + 0x210CC00), Misc::OnReload, (LPVOID*)&OnReloadOG);
	MH_EnableHook((LPVOID)(Base + 0x210CC00));
	MH_CreateHook((LPVOID)(Base + 0x3E04860), Misc::UpdateControlRotation, (LPVOID*)&UpdateControlRotationOG);
	MH_EnableHook((LPVOID)(Base + 0x3E04860));
	MH_CreateHook((LPVOID)(Base + 0x3E495A0), Misc::OnActorBump, (LPVOID*)&OnActorBumpOG);
	MH_EnableHook((LPVOID)(Base + 0x3E495A0));
	MH_CreateHook((LPVOID)(Base + 0x1466490), Misc::GatherNearestLoot, (LPVOID*)&GatherNearestLootOG);
	MH_EnableHook((LPVOID)(Base + 0x1466490));
	MH_CreateHook((LPVOID)(Base + 0x547640), Misc::UserMathErrorFunc, nullptr);
	MH_EnableHook((LPVOID)(Base + 0x547640));

	MH_CreateHook((LPVOID)(Base + Offsets::ProcessEvent), Misc::PE, (LPVOID*)&PEOG);
	MH_EnableHook((LPVOID)(Base + Offsets::ProcessEvent));

	MH_CreateHook((*(void***)UEngine::GetEngine())[0x53], Misc::GetMaxTickRate, nullptr);
	MH_EnableHook((*(void***)UEngine::GetEngine())[0x53]);
}

__int64 Misc::UserMathErrorFunc(__int64 a1)
{
	static auto BaseAddr = __int64(GetModuleHandleW(0));
	//cout << (__int64(_ReturnAddress()) - BaseAddr) << endl;
	if ((__int64(_ReturnAddress()) - BaseAddr) == 0x146E480)
		printf("yoo\n");
	return 0;
}

void Misc::GatherNearestLoot(__int64 Eval)
{
	//UFortAthenaAIBotEvaluator_Loot
	UBehaviorTreeComponent* Comp = *(UBehaviorTreeComponent**)(Eval + 0x48);
	UBlackboardComponent* BB = *(UBlackboardComponent**)(__int64(Comp) + 0xB8);
	UFortServerBotManagerAthena* BotManager = *(UFortServerBotManagerAthena**)(Eval + 0x278);
	AFortAthenaAIBotController* CachedPC = *(AFortAthenaAIBotController**)(Eval + 0x50);
	UFortAthenaAIBotLootingDigestedSkillSet* SkillSet = *(UFortAthenaAIBotLootingDigestedSkillSet**)(Eval + 0x288);
	//cout << Comp->GetName() << endl;
	//cout << BB->GetName() << endl;
	//cout << BotManager->GetName() << endl;
	//cout << CachedPC->GetName() << endl;
	//cout << SkillSet->GetName() << endl;
	*(UFortServerBotManagerAthena**)(Eval + 0x278) = ((AFortGameModeAthena*)UWorld::GetWorld()->AuthorityGameMode)->ServerBotManager;
	return GatherNearestLootOG(Eval);
}

void Misc::OnActorBump(UFortAthenaAIBotPathFollowingComponent* Comp, AActor* SelfActor, AActor* OtherActor, const FVector& NormalImpulse, FHitResult& Hit)
{
	for (auto& bot : SpawnedBots)
	{
		if (bot->PC == Comp->BotController)
		{
			bot->OnActorBump(OtherActor, Hit);
			break;
		}
	}
	return OnActorBumpOG(Comp, SelfActor, OtherActor, NormalImpulse, Hit);
}

FVector GetPawnViewLocation(APawn* Pawn)
{
	return Pawn->K2_GetActorLocation() + FVector(0.f, 0.f, Pawn->BaseEyeHeight);
}

void Misc::UpdateControlRotation(AAIController* PC, float DeltaTime, bool bUpdatePawn)
{
	APawn* const MyPawn = PC->Pawn;
	if (MyPawn)
	{
		FRotator NewControlRotation = PC->GetControlRotation();

		// Look toward focus
		const FVector FocalPoint = PC->GetFocalPoint();
		NewControlRotation = ToOrientationRotator((FocalPoint - GetPawnViewLocation(MyPawn)));

		//// Don't pitch view unless looking at another pawn
		//if (NewControlRotation.Pitch != 0 && Cast<APawn>(GetFocusActor()) == nullptr)//FUCK YOU EPIC GAMES
		//{
		//	NewControlRotation.Pitch = 0.f;
		//}

		PC->SetControlRotation(NewControlRotation);

		if (bUpdatePawn)
		{
			const FRotator CurrentPawnRotation = MyPawn->K2_GetActorRotation();
			
			//oid OnActorBump(class AActor* SelfActor, class AActor* OtherActor, const struct FVector& NormalImpulse, struct FHitResult& Hit
			if (CurrentPawnRotation != NewControlRotation)
			{
				if (MyPawn->bUseControllerRotationPitch || MyPawn->bUseControllerRotationYaw || MyPawn->bUseControllerRotationRoll)
				{
					const FRotator CurrentRotation = MyPawn->K2_GetActorRotation();

					if (!MyPawn->bUseControllerRotationPitch)
					{
						NewControlRotation.Pitch = CurrentRotation.Pitch;
					}

					if (!MyPawn->bUseControllerRotationYaw)
					{
						NewControlRotation.Yaw = CurrentRotation.Yaw;
					}

					if (!MyPawn->bUseControllerRotationRoll)
					{
						NewControlRotation.Roll = CurrentRotation.Roll;
					}

					PC->K2_SetActorRotation(NewControlRotation, true);
				}
			}
		}
	}
}

void __fastcall Misc::OnReload(AFortWeapon* a1, unsigned int a2)
{
	if (!a1)
		return OnReloadOG(a1, a2);
	
	auto Pawn = (AFortPlayerPawnAthena*)a1->GetOwner();

	if (!Pawn || !Pawn->CurrentWeapon)
		return OnReloadOG(a1, a2);
		
	auto PC = (AFortPlayerControllerAthena*)Pawn->Controller;

	if (!PC || !PC->IsA(AFortPlayerControllerAthena::StaticClass()) || !PC->WorldInventory || PC->bInfiniteAmmo)
		return OnReloadOG(a1, a2);
	
	auto ItemDefiniton = a1->WeaponData;
	auto AmmoDef = ItemDefiniton->GetAmmoWorldItemDefinition_BP();

	if (!ItemDefiniton || !AmmoDef)
		return OnReloadOG(a1, a2);
	
	bool Remove = false;

	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition == AmmoDef)
		{
			PC->WorldInventory->Inventory.ReplicatedEntries[i].Count -= a2;
			PC->WorldInventory->Inventory.MarkItemDirty(PC->WorldInventory->Inventory.ReplicatedEntries[i]);
			PC->WorldInventory->HandleInventoryLocalUpdate();
			Inventory::UpdateEntry(PC, PC->WorldInventory->Inventory.ReplicatedEntries[i]);
			if (PC->WorldInventory->Inventory.ReplicatedEntries[i].Count <= 0)
			{
				Inventory::RemoveItem(PC, PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid);
			}
			break;
		}
	}

	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid == Pawn->CurrentWeapon->ItemEntryGuid)
		{
			PC->WorldInventory->Inventory.ReplicatedEntries[i].LoadedAmmo += a2;
			PC->WorldInventory->Inventory.MarkItemDirty(PC->WorldInventory->Inventory.ReplicatedEntries[i]);
			PC->WorldInventory->HandleInventoryLocalUpdate();
			Inventory::UpdateEntry(PC, PC->WorldInventory->Inventory.ReplicatedEntries[i]);
			break;
		}
	}

	return OnReloadOG(a1, a2);
}

void __fastcall Misc::Storm(__int64 a1, int a2)
{
	auto GM = ((AFortGameModeAthena*)UWorld::GetWorld()->AuthorityGameMode);

	for (size_t i = 0; i < GM->AlivePlayers.Num(); i++)
	{
		Accolades::GiveAccolade(GM->AlivePlayers[i], Utils::StaticLoadObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeID_SurviveStormCircle.AccoladeID_SurviveStormCircle"));
	}

	return StormOG(a1, a2);
}

void Misc::OnPossessedPawnDied(AAIController* PC, AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector Momentum)
{
	if (!InstigatedBy)
		return OnPossessedPawnDiedOG(PC, DamagedActor, Damage, InstigatedBy, DamageCauser, HitLocation, FHitComponent, BoneName, Momentum);

	for (auto& bot : SpawnedBots)
	{
		if (bot && bot->PC && bot->TickEnabled && bot->PC == PC)
		{
			bot->OnDied((AFortPlayerStateAthena*)InstigatedBy->PlayerState);
			break;
		}
	}

	return OnPossessedPawnDiedOG(PC, DamagedActor, Damage, InstigatedBy, DamageCauser, HitLocation, FHitComponent, BoneName, Momentum);
}

__int64 Misc::PE(UObject* Obj, UFunction* Func, void* Params)
{
	if (Func->GetName().contains("SendCustomStatEvent") || Func->GetName().contains("SendComplexCustomStatEvent"))
	//if (Func->GetName().contains("hit") || Func->GetName().contains("Hit") || Func->GetName().contains("Overlap"))
	{
		Utils::Log(Func->GetFullName());
		Utils::Log(Obj->GetFullName());
		cout << __int64(Func->ExecFunction) - __int64(GetModuleHandleW(0)) << endl;
	}

	return PEOG(Obj, Func, Params);
}

char __fastcall Misc::Inventory(AActor* a1, __int64 a2)
{
	*(AActor**)(__int64(a1) + 0x488) = Utils::SpawnActor<AFortInventory>({}, {}, a1);
	return InventoryOG(a1, a2);
}

void Misc::ServerMove(AActor* real, FReplicatedPhysicsPawnState InState)
{
	UPrimitiveComponent* Mesh = (UPrimitiveComponent*)real->RootComponent;

	InState.Rotation.X -= 2.5;
	InState.Rotation.Y /= 0.3;
	InState.Rotation.Z -= -2.0;
	InState.Rotation.W /= -1.2;

	FTransform Transform{};
	Transform.Translation = InState.Translation;
	Transform.Rotation = InState.Rotation;
	Transform.Scale3D = FVector{ 1, 1, 1 };

	Mesh->K2_SetWorldTransform(Transform, false, nullptr, true);
	Mesh->bComponentToWorldUpdated = true;
	Mesh->SetPhysicsLinearVelocity(InState.LinearVelocity, 0, FName());
	Mesh->SetPhysicsAngularVelocity(InState.AngularVelocity, 0, FName());
}

// floor loot
void __fastcall Misc::ABuildingSMActor_PostUpdate(ABuildingSMActor* Actor)
{
	if (Actor->IsA(ABuildingContainer::StaticClass()) && ((ABuildingContainer*)Actor)->bStartAlreadySearched_Athena == 1)
	{
		Utils::SpawnFloorLoot((ABuildingContainer*)Actor);
	}

	return ABuildingSMActor_PostUpdateOG(Actor);
}

char Misc::GetSquadIdForCurrentPlayer(AFortGameSessionDedicatedAthena* GameSession, FUniqueNetIdRepl UniqueId)
{
	TArray<AActor*> PCs;
	UGameplayStatics::GetDefaultObj()->GetAllActorsOfClass(UWorld::GetWorld(), AFortPlayerController::StaticClass(), &PCs);
	
	for (int i = 0; i < PCs.Num(); i++)
	{
		auto PC = (AFortPlayerController*)PCs[i];
		auto PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;

		if (PlayerState && PlayerState->AreUniqueIDsIdentical(PC->PlayerState->UniqueId, UniqueId))
		{
			return PlayerState->TeamIndex - 2;
		}
	}
	
	PCs.FreeArray();
	return 0;
}

void Misc::NetMulticast_Athena_BatchedDamageCues(AFortPawn* Pawn, void* a2, void* a3)
{
	if (!Pawn || Pawn->Controller->IsA(AAIController::StaticClass()))
		return;
	
	auto PC = (AFortPlayerController*)Pawn->Controller;
	auto CurrentWeapon = Pawn->CurrentWeapon;
	
	if (PC && CurrentWeapon)
	{
		for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
		{
			auto& ItemEntry = PC->WorldInventory->Inventory.ReplicatedEntries[i];
			
			if (ItemEntry.ItemGuid == CurrentWeapon->ItemEntryGuid)
			{
				ItemEntry.LoadedAmmo = Pawn->CurrentWeapon->AmmoCount;
				Inventory::UpdateEntry(PC, ItemEntry);
				
				PC->WorldInventory->HandleInventoryLocalUpdate();
				PC->WorldInventory->Inventory.MarkItemDirty(ItemEntry);
				break;
			}
		}
	}
	
	return NetMulticast_Athena_BatchedDamageCuesOG(Pawn, a2, a3);
}

float Misc::GetMaxTickRate()
{
	return 30.f;
}

void __fastcall Misc::ServerOnExitVehicle(AFortPlayerPawn* Pawn, unsigned __int8 a2)
{
	if (Pawn->GetVehicleSeatIndex() == 0)
	{
		// this removes not the vehicle weapon but the item you were holding before you entered the vehicle so yeah commented out for now
		 Inventory::RemoveItem((AFortPlayerController*)Pawn->Controller, Pawn->CurrentWeapon->GetInventoryGUID());
		
		auto PC = ((AFortPlayerControllerAthena*)Pawn->Controller);
		for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
		{
			if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition == PC->SwappingItemDefinition)
			{
				Pawn->EquipWeaponDefinition((UFortWeaponItemDefinition*)PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition, PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid);
				break;
			}
		}
	}
	ServerOnExitVehicleOG(Pawn, a2);
}

void Misc::MovingEmoteStopped(AFortPawn* Pawn, void* a2, void* a3)
{
	Pawn->bMovingEmote = false;
	Pawn->bMovingEmoteForwardOnly = false;
	
	return MovingEmoteStoppedOG(Pawn, a2, a3);
}

void Misc::OnCapsuleBeginOverlap(AFortPlayerPawnAthena* a1, UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, FHitResult SweepResult)
{
	if (a1->Controller->IsA(AFortAthenaAIBotController::StaticClass()))
		return OnCapsuleBeginOverlapOG(a1, OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	Utils::Log(OtherActor->GetName());
	Utils::Log(OtherComp->GetName());

	static UClass* CoinClass = Utils::StaticLoadObject<UClass>("/Game/Athena/Items/QuestInteractables/ItemCollection/BP_ItemCollection_Touch.BP_ItemCollection_Touch_C");
	static UClass* BoatTimedClass = Utils::StaticLoadObject<UClass>("/Game/Athena/Items/QuestInteractables/TimedChallenge/BoatTrial/Prop_QuestInteractable_TimeTrials_Boat.Prop_QuestInteractable_TimeTrials_Boat_C");
	static UClass* FireClass = Utils::StaticLoadObject<UClass>("/Game/Athena/Items/QuestInteractables/FlamingHoops/Boat/Actor_QuestObject_Touch_FlamingHoopsBoat_Parent.Actor_QuestObject_Touch_FlamingHoopsBoat_Parent_C");

	if (OtherActor->IsA(CoinClass) && OtherComp->GetName() == "CollectOverlap")
	{
		Utils::Log("XP Coin!");
		AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)a1->Controller;
		
		static UProperty* BackendNameProp = Utils::StaticLoadObject<UProperty>("/Game/Athena/Items/QuestInteractables/ItemCollection/BP_ItemCollection_Touch.BP_ItemCollection_Touch_C.QuestBackendName");
		static UProperty* QuestItemProp = Utils::StaticLoadObject<UProperty>("/Game/Athena/Items/QuestInteractables/ItemCollection/BP_ItemCollection_Touch.BP_ItemCollection_Touch_C.CollectItemQuest");

		FName BackendName = *(FName*)(__int64(OtherActor) + BackendNameProp->Offset);

		UFortQuestItemDefinition* QuestDef = *(UFortQuestItemDefinition**)(__int64(OtherActor) + QuestItemProp->Offset);
		if (!QuestDef)
			return;
		Quests::ProgressQuest(PC, QuestDef, BackendName);
		OtherActor->ProcessEvent(OtherActor->Class->GetFunction("BP_ItemCollection_Touch_C", "ParentQuestUpdated"), nullptr);
	}
	else if (a1->IsInVehicle() && OtherActor->IsA(BoatTimedClass))
	{
		string OverlapName = OtherComp->GetName();
		if (OverlapName.contains("CP"))
		{
			string Number = Utils::SplitString(true, "CP", OverlapName);
			string Ok = Utils::SplitString(false, (OverlapName.contains("Collision") ? "Collision" : "Coll"), Number);
			int Idx = stoi(Ok) - 1;
			
			Utils::Log("Index is " + to_string(Idx));

			OtherActor->ProcessEvent(OtherActor->Class->GetFunction("Prop_QuestInteractable_TimeTrials_Boat_C", "GateHit"), &Idx);
			
			if (Idx == 7)
			{
				static UProperty* QuestItemProp = Utils::StaticLoadObject<UProperty>("/Game/Athena/Items/QuestInteractables/TimedChallenge/BoatTrial/Prop_QuestInteractable_TimeTrials_Boat.Prop_QuestInteractable_TimeTrials_Boat_C.QuestItem");
				static UProperty* QuestBackendObjectiveName = Utils::StaticLoadObject<UProperty>("/Game/Athena/Items/QuestInteractables/TimedChallenge/BoatTrial/Prop_QuestInteractable_TimeTrials_Boat.Prop_QuestInteractable_TimeTrials_Boat_C.QuestBackendObjectiveName");

				auto Backendname = *(FName*)(__int64(OtherActor) + QuestBackendObjectiveName->Offset);
				auto Quest = *(UFortQuestItemDefinition**)(__int64(OtherActor) + QuestItemProp->Offset);
				auto QuestManager = ((AFortPlayerControllerAthena*)a1->Controller)->GetQuestManager(ESubGame::Athena);

				if (!QuestManager->GetQuestWithDefinition(Quest)->HasCompletedObjectiveWithName(Backendname))
				{
					Quests::ProgressQuest((AFortPlayerControllerAthena*)a1->Controller, Quest, Backendname);
					
					OtherActor->ProcessEvent(OtherActor->Class->GetFunction("Prop_QuestInteractable_TimeTrials_Boat_C", "PlaySuccessFX"), &a1->Controller);
					OtherActor->ProcessEvent(OtherActor->Class->GetFunction("Prop_QuestInteractable_TimeTrials_Boat_C", "ParentQuestUpdated"), nullptr);
					OtherActor->ProcessEvent(OtherActor->Class->GetFunction("Prop_QuestInteractable_TimeTrials_Boat_C", "AwardQuest"), &a1);
				}
			}
		}
	}
	else if (a1->IsInVehicle() && OtherActor->IsA(FireClass))
	{
		static UProperty* QuestItemProp = Utils::StaticLoadObject<UProperty>("/Game/Athena/Items/QuestInteractables/Generic/Actor_QuestObject_Touch_Parent.Actor_QuestObject_Touch_Parent_C.QuestItem");
		static UProperty* QuestBackendObjectiveName = Utils::StaticLoadObject<UProperty>("/Game/Athena/Items/QuestInteractables/Generic/Actor_QuestObject_Touch_Parent.Actor_QuestObject_Touch_Parent_C.ObjBackendName");

		auto Backendname = *(FName*)(__int64(OtherActor) + QuestBackendObjectiveName->Offset);
		auto Quest = *(UFortQuestItemDefinition**)(__int64(OtherActor) + QuestItemProp->Offset);
		auto QuestManager = ((AFortPlayerControllerAthena*)a1->Controller)->GetQuestManager(ESubGame::Athena);

		if (!QuestManager->GetQuestWithDefinition(Quest)->HasCompletedObjectiveWithName(Backendname))
		{
			Quests::ProgressQuest((AFortPlayerControllerAthena*)a1->Controller, Quest, Backendname);
			OtherActor->ProcessEvent(OtherActor->Class->GetFunction("Actor_QuestObject_Touch_FlamingHoops_Parent_C", "ObjectiveSuccessfullyCompleted"), &a1->Controller);
		}
	}

	return OnCapsuleBeginOverlapOG(a1, OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

__int64 __fastcall Misc::DispatchRequest(__int64 a1, unsigned __int64* a2, int a3)
{
	return DispatchRequestOG(a1,a2,3);
}

__int64 __fastcall Misc::Context(__int64)
{
	return 1;
}

__int64 __fastcall Misc::CollectGarbageInternal(unsigned int, unsigned __int8)
{
	return 0;
}