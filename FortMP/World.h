#pragma once
#include "Utils.h"

namespace World
{
	void InitHooks();
	SDK::APlayerController* SpawnPlayActor(SDK::UWorld*,SDK::UPlayer*,SDK::ENetRole,const SDK::FURL&, void*, SDK::FString&, uint8);
	__int64 GetNetMode(void*);
}