#pragma once
#include "Utils.h"

namespace NetDriver
{
	void InitHooks();
	void TickFlush(SDK::UNetDriver*,float);
	char __fastcall kick(__int64, __int64, __int64);
}