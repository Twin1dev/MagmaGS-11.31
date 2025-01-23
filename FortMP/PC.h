#pragma once
#include "Utils.h"

namespace PC
{
	void InitHooks();
	void HandleStartingNewPlayer(AFortGameModeAthena* GM, APlayerController* NewPlayer);
	void EnterAircraft(AFortPlayerControllerAthena* PC, AActor* Aircraft);
	void ServerAttemptAircraftJump(UFortControllerComponent_Aircraft*, FRotator);
	void LoadingScreenDropped(AFortPlayerControllerAthena*);
	void ServerAcknowledgePossession(AFortPlayerControllerAthena*, APawn*);
	void ServerExecuteInventoryItem(AFortPlayerControllerAthena*, FGuid);
	void GetPlayerViewPoint(AFortPlayerControllerAthena* PC, FVector& Loc, FRotator& Rot);
	void ServerCheat(AFortPlayerControllerAthena*, FString);
	void ServerPlayEmoteItem(AFortPlayerControllerAthena* PC, UFortItemDefinition* EmoteAsset, float);
	void ServerPlaySquadQuickChatMessage(AFortPlayerControllerAthena* PlayerController, FAthenaQuickChatActiveEntry ChatEntry, __int64);
	void ServerAttemptInteract(UFortControllerComponent_Interaction* Comp, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, ETInteractionType InteractType, UObject* OptionalData, EInteractionBeingAttempted InteractionBeingAttempted);
	__int64 OnDamageServer(ABuildingSMActor* Actor, float Damage, FGameplayTagContainer DamageTags, FVector Momentum, FHitResult HitInfo, AFortPlayerControllerAthena* InstigatedBy, AActor* DamageCauser, FGameplayEffectContextHandle EffectContext);
	void ServerCreateBuildingActor(AFortPlayerControllerAthena* PC, FCreateBuildingActorData BuildingData);
	void DestroyPickup(AFortPickup* Pickup);
	void MovingEmoteStopped(AFortPawn* Pawn, void*, void*);
	void ClientOnPawnDied(AFortPlayerControllerAthena*, FFortPlayerDeathReport);
	void ServerRepairBuildingActor(AFortPlayerController* PlayerController, ABuildingSMActor* BuildingActorToRepair);
	void ServerHandlePickup(AFortPlayerPawnAthena* Pawn, AFortPickup* Pickup, float InFlyTime, const FVector& InStartDirection, bool bPlayPickupSound);
	void ServerSendZiplineState(AFortPlayerPawn* Pawn, FZiplinePawnState InZiplineState);
	void ServerEditBuildingActor(AFortPlayerControllerAthena* PC, ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, uint8 RotationIterations, bool bMirrored);
	void ServerEndEditingBuildingActor(AFortPlayerControllerAthena* PC, ABuildingSMActor* BuildingActorToEdit);
	void ServerBeginEditingBuildingActor(AFortPlayerControllerAthena* PC, ABuildingSMActor* BuildingActorToEdit);
	void ServerAttemptInventoryDrop(AFortPlayerControllerAthena* PC, const FGuid& ItemGuid, int32 Count, bool bTrash);
	void ServerReviveFromDBNO(AFortPlayerPawnAthena* Pawn, AFortPlayerControllerAthena* EventInstigator);
	void ServerAddMapMarker(UAthenaMarkerComponent* Comp, const FFortClientMarkerRequest& MarkerRequest);
	void ServerRemoveMapMarker(UAthenaMarkerComponent* Comp, FMarkerID MarkerID, ECancelMarkerReason CancelReason);
	void ServerSetInAircraft(AFortPlayerStateAthena* PlayerState, bool a2);
}