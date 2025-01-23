#pragma once
#include "Utils.h"
#include "SDK.hpp"

namespace Gamemode
{
	inline bool Ready = false;
	void InitHooks();
	bool ReadyToStartMatch(SDK::AFortGameModeAthena* GM);
	SDK::APawn* SpawnDefaultPawnFor(SDK::AGameMode*,SDK::AController*,SDK::AActor*);
	__int64 __fastcall OnAircraftEnteredDropZone(AFortGameModeAthena*);
	__int64 __fastcall OnAircraftExitedDropZone(__int64, AFortAthenaAircraft*);
}