#pragma once
#include "Utils.h"

using namespace SDK;

namespace Misc
{
	void InitHooks();
	__int64 __fastcall CollectGarbageInternal(unsigned int, unsigned __int8);
	__int64 __fastcall Context(__int64);
	__int64 __fastcall DispatchRequest(__int64 a1, unsigned __int64* a2, int a3);
	void MovingEmoteStopped(AFortPawn* Pawn, void* a2, void* a3);
	void OnCapsuleBeginOverlap(AFortPlayerPawnAthena* a1, UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, FHitResult SweepResult);
	void __fastcall ServerOnExitVehicle(AFortPlayerPawn* Pawn, unsigned __int8 a2);
	float GetMaxTickRate();
	void ServerMove(AActor* real, FReplicatedPhysicsPawnState);
	char __fastcall Inventory(AActor* a1, __int64 a2);
	__int64 PE(UObject* Obj, UFunction* Func, void* Params);
	void OnPossessedPawnDied(AAIController* PC, AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector Momentum);
	void __fastcall Storm(__int64, int);
	void __fastcall ABuildingSMActor_PostUpdate(ABuildingSMActor*);
	char GetSquadIdForCurrentPlayer(AFortGameSessionDedicatedAthena* GameSession, FUniqueNetIdRepl UniqueId);
	void NetMulticast_Athena_BatchedDamageCues(AFortPawn* Pawn, void* a2, void* a3);
	void __fastcall OnReload(AFortWeapon* a1, unsigned int a2);
	void UpdateControlRotation(AAIController* PC, float DeltaTime, bool bUpdatePawn);
	void OnActorBump(UFortAthenaAIBotPathFollowingComponent* Comp, AActor* SelfActor, AActor* OtherActor, const FVector& NormalImpulse, FHitResult& Hit);
	void GatherNearestLoot(__int64 Eval);
	__int64 UserMathErrorFunc(__int64 a1);
}