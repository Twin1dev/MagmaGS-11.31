#include "Utils.h"
#include "minhook/MinHook.h"
#include <cstdlib>
#include <ctime>
#include "real.h"

#pragma comment(lib, "minhook/minhook.lib")

using namespace std;
using namespace SDK;

#define _LOGGING true
#if _LOGGING
#define LOG(str) printf("[FortMp]: %s\n", str)
#else
#define LOG() // This isn't logging because _LOGGING is set to false
#endif


void Utils::Log(const char* Str)
{
    LOG(Str);
}

void Utils::Log(std::string Str)
{
    LOG(Str.c_str());
}

UFortEngine* Utils::GetEngine()
{
	static auto engine = UObject::FindObject<UFortEngine>("FortEngine_");
	return engine;
}

UWorld* Utils::GetWorld()
{
	return GetEngine()->GameViewport->World;
}

void Utils::SwapVTable(void* base, int Idx, void* Detour, void** OG)
{
    if (!base)
        return;

    void** VTable = *(void***)base;
    if (!VTable || !VTable[Idx])
        return;
    if (OG)
    {
        *OG = VTable[Idx];
    }

    DWORD oldProtection;

    VirtualProtect(&VTable[Idx], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtection);

    VTable[Idx] = Detour;

    VirtualProtect(&VTable[Idx], sizeof(void*), oldProtection, NULL);
}

void Utils::SpawnPickup(FVector Loc, UFortItemDefinition* Def, EFortPickupSourceTypeFlag Flag, EFortPickupSpawnSource SpawnSource, int Count, int LoadedAmmo, AFortPawn* Owner)
{
    AFortPickupAthena* Pickup = Utils::SpawnActor<AFortPickupAthena>(Loc);
    Pickup->bRandomRotation = true;
    Pickup->PrimaryPickupItemEntry.ItemDefinition = Def;
    Pickup->PrimaryPickupItemEntry.Count = Count;
    Pickup->PrimaryPickupItemEntry.LoadedAmmo = LoadedAmmo;
    Pickup->OnRep_PrimaryPickupItemEntry();

    if (Flag == EFortPickupSourceTypeFlag::Container)
    {
        Pickup->bTossedFromContainer = true;
        Pickup->OnRep_TossedFromContainer();
    }

    Pickup->TossPickup(Loc, nullptr, -1, true, false, Flag, SpawnSource);
}

void Inventory::InitInventoryForPlayer(SDK::AFortPlayerController* PC) // PC: player controller
{
    if (!PC->WorldInventory)
        PC->WorldInventory = Utils::SpawnActor<AFortInventory>({}, {}, PC);
    
    if (!PC->ClientQuickBars)
        PC->ClientQuickBars = Utils::SpawnActor<AFortQuickBars>({}, {}, PC);
    
    PC->HandleWorldInventoryLocalUpdate();

    auto GM = (AFortGameModeAthena*)UWorld::GetWorld()->AuthorityGameMode; // GM: game mode

    for (size_t i = 0; i < GM->StartingItems.Num(); i++)
    {
        if (GM->StartingItems[i].Count <= 0) // How can it be <= 0 lol
            continue;
        Inventory::GiveItem(PC, GM->StartingItems[i].Item, GM->StartingItems[i].Count);
    }

    if (PC->CosmeticLoadoutPC.Pickaxe)
    {
        Inventory::GiveItem(PC, PC->CosmeticLoadoutPC.Pickaxe->WeaponDefinition);
    }
}


string Utils::SplitString(bool SecondString, string delim, string strtosplit)
{
    auto start = 0U;
    auto end = strtosplit.find(delim);
    if (SecondString)
    {
        while (end != std::string::npos)
        {
            start = end + delim.length();
            end = strtosplit.find(delim, start);
        }
    }

    return strtosplit.substr(start, end);
}

char __fastcall Utils::SpawnLoot(ABuildingContainer* Object)
{
    if (Object->bAlreadySearched)
        return 0;

    Object->bAlreadySearched = true;
    Object->SearchBounceData.SearchAnimationCount++;

    FVector Loc = Object->K2_GetActorLocation() + (Object->GetActorForwardVector() * Object->LootSpawnLocation_Athena.X) + (Object->GetActorRightVector() * Object->LootSpawnLocation_Athena.Y) + (Object->GetActorUpVector() * Object->LootSpawnLocation_Athena.Z);
    Object->BounceContainer();
    Object->OnRep_bAlreadySearched();

    TArray<UFortItemDefinition*> AssaultRifleDefinitions;
    TArray<UFortItemDefinition*> ShotgunDefinitions;
    TArray<UFortItemDefinition*> PistolDefinitions;
    TArray<UFortItemDefinition*> ConsumableDefinitions;

    // Assault Rifles
    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_C_Ore_T02.WID_Assault_SemiAuto_Athena_C_Ore_T02"));
    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_C_Ore_T02.WID_Assault_Auto_Athena_C_Ore_T02"));
    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_UC_Ore_T03.WID_Assault_SemiAuto_Athena_UC_Ore_T03"));
    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_UC_Ore_T03.WID_Assault_Auto_Athena_UC_Ore_T03"));
    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_R_Ore_T03.WID_Assault_SemiAuto_Athena_R_Ore_T03"));
    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_R_Ore_T03.WID_Assault_Auto_Athena_R_Ore_T03"));
    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_VR_Ore_T03.WID_Assault_SemiAuto_Athena_VR_Ore_T03"));
    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03"));
    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_SR_Ore_T03.WID_Assault_SemiAuto_Athena_SR_Ore_T03"));
   AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03"));


    // Shotguns
    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_C_Ore_T03.WID_Shotgun_Standard_Athena_C_Ore_T03"));
    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_UC_Ore_T03.WID_Shotgun_SemiAuto_Athena_UC_Ore_T03"));
    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03"));
    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_R_Ore_T03.WID_Shotgun_SemiAuto_Athena_R_Ore_T03"));
    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_VR_Ore_T03.WID_Shotgun_Standard_Athena_VR_Ore_T03"));
    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03"));
    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_SR_Ore_T03.WID_Shotgun_Standard_Athena_SR_Ore_T03"));
    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03.WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03"));
    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_HighSemiAuto_Athena_SR_Ore_T03.WID_Shotgun_HighSemiAuto_Athena_SR_Ore_T03"));

    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_C_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_C_Ore_T03"));
    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_C_Ore_T02.WID_Pistol_SemiAuto_Athena_C_Ore_T02"));
    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_UC_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_UC_Ore_T03"));
    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_UC_Ore_T03.WID_Pistol_SemiAuto_Athena_UC_Ore_T03"));
    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03"));
    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_R_Ore_T03.WID_Pistol_SemiAuto_Athena_R_Ore_T03"));
    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03"));
    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_Standard_Athena_VR.WID_Pistol_Standard_Athena_VR"));
    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_SR_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_SR_Ore_T03"));
    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_Standard_Athena_SR.WID_Pistol_Standard_Athena_SR"));


    ConsumableDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall"));
    ConsumableDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Medkit/Athena_Medkit.Athena_Medkit"));
    ConsumableDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Bandage/Athena_Bandage.Athena_Bandage"));
    ConsumableDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields"));


    if (AssaultRifleDefinitions.Num() == 0 || ShotgunDefinitions.Num() == 0)
    {
        return 0;
    }

    int32 TotalAssaultRifleWeight = 0;
    int32 NumRarities = AssaultRifleDefinitions.Num();
    for (int32 Index = 0; Index < NumRarities; ++Index)
    {
        if (Index >= NumRarities - 4) {
            TotalAssaultRifleWeight += 100; 
        }
        else {
            TotalAssaultRifleWeight += NumRarities - Index;
        }
    }

    int32 TotalShotgunWeight = 0;
    NumRarities = ShotgunDefinitions.Num();
    for (int32 Index = 0; Index < NumRarities; ++Index)
    {
        if (Index >= NumRarities - 4) {
            TotalShotgunWeight += 100;
        }
        else {
            TotalShotgunWeight += NumRarities - Index;
        }
    }



    int32 TotalPistolWeight = 0;
    NumRarities = PistolDefinitions.Num();
    for (int32 Index = 0; Index < NumRarities; ++Index)
    {
        if (Index >= NumRarities - 4) {
            TotalPistolWeight += 100; 
        }
        else {
            TotalPistolWeight += NumRarities - Index;
        }
    }


    int32 TotalConsumableWeight = 0;
    NumRarities = ConsumableDefinitions.Num();
    for (int32 Index = 0; Index < NumRarities; ++Index)
    {
        TotalConsumableWeight += (NumRarities - Index) * 2; 
    }


    int32 TotalWeight = TotalAssaultRifleWeight + TotalShotgunWeight + TotalPistolWeight + TotalConsumableWeight;
    int32 RandomWeight = std::rand() % TotalWeight;

    if (RandomWeight < TotalAssaultRifleWeight)
    {
        int32 AssaultRifleIndex = std::rand() % AssaultRifleDefinitions.Num();
        UFortItemDefinition* RandomItemDefinition = AssaultRifleDefinitions[AssaultRifleIndex];
        Utils::SpawnPickup(Loc, RandomItemDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, 1, 30);

        UFortItemDefinition* AmmoItemDefinition = Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium");
        if (AmmoItemDefinition)
        {
            Utils::SpawnPickup(Loc, AmmoItemDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, 20, 30);
        }

        TArray<ConsumableInfo> ConsumableDefinitions = InitializeConsumableDefinitions();

        int32 ConsumableIndex = std::rand() % ConsumableDefinitions.Num();
        ConsumableInfo& RandomConsumableInfo = ConsumableDefinitions[ConsumableIndex];
        UFortItemDefinition* RandomConsumDefinition = RandomConsumableInfo.Definition;
        int Quantity = RandomConsumableInfo.Quantity;
        Utils::SpawnPickup(Loc, RandomConsumDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, Quantity, 1);

    }
    else if (RandomWeight < TotalAssaultRifleWeight + TotalShotgunWeight)
    {
        int32 ShotgunIndex = std::rand() % ShotgunDefinitions.Num();
        UFortItemDefinition* RandomItemDefinition = ShotgunDefinitions[ShotgunIndex];
        Utils::SpawnPickup(Loc, RandomItemDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, 1, 5);

        UFortItemDefinition* AmmoItemDefinition = Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataShells.AthenaAmmoDataShells");
        if (AmmoItemDefinition)
        {
            Utils::SpawnPickup(Loc, AmmoItemDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, 5, 5);
        }

        TArray<ConsumableInfo> ConsumableDefinitions = InitializeConsumableDefinitions();

        int32 ConsumableIndex = std::rand() % ConsumableDefinitions.Num();
        ConsumableInfo& RandomConsumableInfo = ConsumableDefinitions[ConsumableIndex];
        UFortItemDefinition* RandomConsumDefinition = RandomConsumableInfo.Definition;
        int Quantity = RandomConsumableInfo.Quantity;
        Utils::SpawnPickup(Loc, RandomConsumDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, Quantity, 1);
    }
    else if (RandomWeight < TotalAssaultRifleWeight + TotalShotgunWeight + TotalPistolWeight)
    {
        int32 PistolIndex = std::rand() % PistolDefinitions.Num();
        UFortItemDefinition* RandomItemDefinition = PistolDefinitions[PistolIndex];
        Utils::SpawnPickup(Loc, RandomItemDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, 1, 28);

        UFortItemDefinition* AmmoItemDefinition = Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight");
        if (AmmoItemDefinition)
        {
            Utils::SpawnPickup(Loc, AmmoItemDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, 5, 18);
        }

        TArray<ConsumableInfo> ConsumableDefinitions = InitializeConsumableDefinitions();

        int32 ConsumableIndex = std::rand() % ConsumableDefinitions.Num();
        ConsumableInfo& RandomConsumableInfo = ConsumableDefinitions[ConsumableIndex];
        UFortItemDefinition* RandomConsumDefinition = RandomConsumableInfo.Definition;
        int Quantity = RandomConsumableInfo.Quantity;
        Utils::SpawnPickup(Loc, RandomConsumDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, Quantity, 1);

    }
    else if (RandomWeight < TotalAssaultRifleWeight + TotalShotgunWeight + TotalPistolWeight + TotalConsumableWeight)
    {

        TArray<ConsumableInfo> ConsumableDefinitions = InitializeConsumableDefinitions();

        int32 ConsumableIndex = std::rand() % ConsumableDefinitions.Num();
        ConsumableInfo& RandomConsumableInfo = ConsumableDefinitions[ConsumableIndex];
        UFortItemDefinition* RandomConsumDefinition = RandomConsumableInfo.Definition;
        int Quantity = RandomConsumableInfo.Quantity;
        Utils::SpawnPickup(Loc, RandomConsumDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, Quantity, 1);
    }
    else
    {

    }

    return 1;

}

char __fastcall Utils::SpawnFloorLoot(ABuildingContainer* Object)
{
    if (Object->bAlreadySearched)
        return 0;

    Object->bAlreadySearched = true;
    Object->SearchBounceData.SearchAnimationCount++;

    FVector Loc = Object->K2_GetActorLocation() + (Object->GetActorForwardVector() * Object->LootSpawnLocation_Athena.X) + (Object->GetActorRightVector() * Object->LootSpawnLocation_Athena.Y) + (Object->GetActorUpVector() * Object->LootSpawnLocation_Athena.Z);
    Object->BounceContainer();
    Object->OnRep_bAlreadySearched();

    TArray<UFortItemDefinition*> AssaultRifleDefinitions;
    TArray<UFortItemDefinition*> ShotgunDefinitions;
    TArray<UFortItemDefinition*> PistolDefinitions;
    TArray<UFortItemDefinition*> ConsumableDefinitions;

    // Assault Rifles
    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_C_Ore_T02.WID_Assault_SemiAuto_Athena_C_Ore_T02"));
    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_C_Ore_T02.WID_Assault_Auto_Athena_C_Ore_T02"));
    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_UC_Ore_T03.WID_Assault_SemiAuto_Athena_UC_Ore_T03"));
    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_UC_Ore_T03.WID_Assault_Auto_Athena_UC_Ore_T03"));
    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_R_Ore_T03.WID_Assault_SemiAuto_Athena_R_Ore_T03"));
    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_R_Ore_T03.WID_Assault_Auto_Athena_R_Ore_T03"));
    //    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_VR_Ore_T03.WID_Assault_SemiAuto_Athena_VR_Ore_T03"));
    //    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03"));
    //    AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_SR_Ore_T03.WID_Assault_SemiAuto_Athena_SR_Ore_T03"));
    //   AssaultRifleDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03"));


        // Shotguns
    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_C_Ore_T03.WID_Shotgun_Standard_Athena_C_Ore_T03"));
    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_UC_Ore_T03.WID_Shotgun_SemiAuto_Athena_UC_Ore_T03"));
    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03"));
    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_R_Ore_T03.WID_Shotgun_SemiAuto_Athena_R_Ore_T03"));
    //    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_VR_Ore_T03.WID_Shotgun_Standard_Athena_VR_Ore_T03"));
    //    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03"));
    //    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_SR_Ore_T03.WID_Shotgun_Standard_Athena_SR_Ore_T03"));
    //    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03.WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03"));
    //    ShotgunDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_HighSemiAuto_Athena_SR_Ore_T03.WID_Shotgun_HighSemiAuto_Athena_SR_Ore_T03"));

    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_C_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_C_Ore_T03"));
    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_C_Ore_T02.WID_Pistol_SemiAuto_Athena_C_Ore_T02"));
    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_UC_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_UC_Ore_T03"));
    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_UC_Ore_T03.WID_Pistol_SemiAuto_Athena_UC_Ore_T03"));
    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03"));
    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_R_Ore_T03.WID_Pistol_SemiAuto_Athena_R_Ore_T03"));
    //    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03"));
    //    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_Standard_Athena_VR.WID_Pistol_Standard_Athena_VR"));
    //    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_SR_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_SR_Ore_T03"));
    //    PistolDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_Standard_Athena_SR.WID_Pistol_Standard_Athena_SR"));


    ConsumableDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall"));
    ConsumableDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Medkit/Athena_Medkit.Athena_Medkit"));
    ConsumableDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Bandage/Athena_Bandage.Athena_Bandage"));
    ConsumableDefinitions.Add(Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields"));



    if (AssaultRifleDefinitions.Num() == 0 || ShotgunDefinitions.Num() == 0)
    {
        return 0;
    }

    int32 TotalAssaultRifleWeight = 0;
    int32 NumRarities = AssaultRifleDefinitions.Num();
    for (int32 Index = 0; Index < NumRarities; ++Index)
    {
        if (Index >= NumRarities - 4) {
            TotalAssaultRifleWeight += 100; 
        }
        else {
            TotalAssaultRifleWeight += NumRarities - Index;
        }
    }

    int32 TotalShotgunWeight = 0;
    NumRarities = ShotgunDefinitions.Num();
    for (int32 Index = 0; Index < NumRarities; ++Index)
    {
        if (Index >= NumRarities - 4) {
            TotalShotgunWeight += 100; 
        }
        else {
            TotalShotgunWeight += NumRarities - Index;
        }
    }




    int32 TotalPistolWeight = 0;
    NumRarities = PistolDefinitions.Num();
    for (int32 Index = 0; Index < NumRarities; ++Index)
    {
        if (Index >= NumRarities - 4) {
            TotalPistolWeight += 100; 
        }
        else {
            TotalPistolWeight += NumRarities - Index;
        }
    }

    int32 TotalConsumableWeight = 0;
    NumRarities = ConsumableDefinitions.Num();
    for (int32 Index = 0; Index < NumRarities; ++Index)
    {
        TotalConsumableWeight += (NumRarities - Index) * 2; 
    }


    int32 TotalWeight = TotalAssaultRifleWeight + TotalShotgunWeight + TotalPistolWeight + TotalConsumableWeight;
    int32 RandomWeight = std::rand() % TotalWeight;

    if (RandomWeight < TotalAssaultRifleWeight)
    {
        int32 AssaultRifleIndex = std::rand() % AssaultRifleDefinitions.Num();
        UFortItemDefinition* RandomItemDefinition = AssaultRifleDefinitions[AssaultRifleIndex];
        Utils::SpawnPickup(Loc, RandomItemDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, 1, 30);

        UFortItemDefinition* AmmoItemDefinition = Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium");
        if (AmmoItemDefinition)
        {
            Utils::SpawnPickup(Loc, AmmoItemDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, 20, 30);
        }
    }
    else if (RandomWeight < TotalAssaultRifleWeight + TotalShotgunWeight)
    {
        int32 ShotgunIndex = std::rand() % ShotgunDefinitions.Num();
        UFortItemDefinition* RandomItemDefinition = ShotgunDefinitions[ShotgunIndex];
        Utils::SpawnPickup(Loc, RandomItemDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, 1, 5);

        UFortItemDefinition* AmmoItemDefinition = Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataShells.AthenaAmmoDataShells");
        if (AmmoItemDefinition)
        {
            Utils::SpawnPickup(Loc, AmmoItemDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, 5, 5);
        }
    }
    else if (RandomWeight < TotalAssaultRifleWeight + TotalShotgunWeight + TotalPistolWeight)
    {
        int32 PistolIndex = std::rand() % PistolDefinitions.Num();
        UFortItemDefinition* RandomItemDefinition = PistolDefinitions[PistolIndex];
        Utils::SpawnPickup(Loc, RandomItemDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, 1, 28);

        UFortItemDefinition* AmmoItemDefinition = Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight");
        if (AmmoItemDefinition)
        {
            Utils::SpawnPickup(Loc, AmmoItemDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, 5, 18);
        }
    }
    else if (RandomWeight < TotalAssaultRifleWeight + TotalShotgunWeight + TotalPistolWeight + TotalConsumableWeight)
    {

        TArray<ConsumableInfo> ConsumableDefinitions = InitializeConsumableDefinitions();

        int32 ConsumableIndex = std::rand() % ConsumableDefinitions.Num();
        ConsumableInfo& RandomConsumableInfo = ConsumableDefinitions[ConsumableIndex];
        UFortItemDefinition* RandomItemDefinition = RandomConsumableInfo.Definition;
        int Quantity = RandomConsumableInfo.Quantity;
        Utils::SpawnPickup(Loc, RandomItemDefinition, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, Quantity, 1);
    }
    else
    {
    }

    return 1;

}



void Inventory::GiveItem(SDK::AFortPlayerController* PC, SDK::UFortItemDefinition* Def, int Count, int LoadedAmmo, bool Stack)
{
    bool Found = false;
    if (!Stack)
    {
        UFortWorldItem* Item = (UFortWorldItem*)Def->CreateTemporaryItemInstanceBP(Count, 0);
        Item->OwnerInventory = PC->WorldInventory;
        Item->SetOwningControllerForTemporaryItem(PC);
        Item->ItemEntry.LoadedAmmo = LoadedAmmo;
        PC->WorldInventory->Inventory.ReplicatedEntries.Add(Item->ItemEntry);
        PC->WorldInventory->Inventory.ItemInstances.Add(Item);
        PC->WorldInventory->Inventory.MarkItemDirty(Item->ItemEntry);
        PC->WorldInventory->HandleInventoryLocalUpdate();
    }
    else
    {
        for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
        {
            if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition == Def)
            {
                Found = true;
                PC->WorldInventory->Inventory.ReplicatedEntries[i].Count += Count;
                Inventory::UpdateEntry(PC, PC->WorldInventory->Inventory.ReplicatedEntries[i]);
                if (PC->WorldInventory->Inventory.ReplicatedEntries[i].Count > Def->MaxStackSize)
                {
                    Utils::SpawnPickup(PC->Pawn->K2_GetActorLocation(), PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition, EFortPickupSourceTypeFlag::Tossed, EFortPickupSpawnSource::Unset, PC->WorldInventory->Inventory.ReplicatedEntries[i].Count - PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition->MaxStackSize, 0, PC->MyFortPawn);
                    PC->WorldInventory->Inventory.ReplicatedEntries[i].Count = Def->MaxStackSize;
                    Inventory::UpdateEntry(PC, PC->WorldInventory->Inventory.ReplicatedEntries[i]);
                }
                PC->WorldInventory->Inventory.MarkItemDirty(PC->WorldInventory->Inventory.ReplicatedEntries[i]);
                PC->WorldInventory->HandleInventoryLocalUpdate();
                break;
            }
        }

        if (!Found)
            Inventory::GiveItem(PC, Def, Count, LoadedAmmo, false);
    }
}

void Inventory::UpdateEntry(AFortPlayerController* PC, FFortItemEntry& Entry)
{
    for (size_t i = 0; i < PC->WorldInventory->Inventory.ItemInstances.Num(); i++)
    {
        if (PC->WorldInventory->Inventory.ItemInstances[i]->ItemEntry.ItemGuid == Entry.ItemGuid)
        {
            PC->WorldInventory->Inventory.ItemInstances[i]->ItemEntry.Count = Entry.Count;
            PC->WorldInventory->Inventory.ItemInstances[i]->ItemEntry.LoadedAmmo = Entry.LoadedAmmo;
            break;
        }
    }
}

void Inventory::RemoveItem(AFortPlayerController* PC, FGuid Guid)
{
    for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
    {
        if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid == Guid)
        {
            PC->WorldInventory->Inventory.ReplicatedEntries.RemoveSingle(i);
            break;
        }
    }

    for (size_t i = 0; i < PC->WorldInventory->Inventory.ItemInstances.Num(); i++)
    {
        if (PC->WorldInventory->Inventory.ItemInstances[i]->ItemEntry.ItemGuid == Guid)
        {
            PC->WorldInventory->Inventory.ItemInstances.RemoveSingle(i);
            break;
        }
    }

    PC->WorldInventory->Inventory.MarkArrayDirty();
    PC->WorldInventory->bRequiresLocalUpdate = true;
    PC->WorldInventory->HandleInventoryLocalUpdate();
}

void Inventory::RemoveItem(SDK::AFortPlayerController* PC, SDK::UFortItemDefinition* Def, int Count)
{
    if (!PC || !PC->WorldInventory)
        return;
    bool Remove = false;
    for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
    {
        if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition == Def)
        {
            PC->WorldInventory->Inventory.ReplicatedEntries[i].Count -= Count;
            Inventory::UpdateEntry(PC, PC->WorldInventory->Inventory.ReplicatedEntries[i]);
            if (PC->WorldInventory->Inventory.ReplicatedEntries[i].Count <= 0)
            {
                Remove = true;
                PC->WorldInventory->Inventory.ReplicatedEntries.RemoveSingle(i);
            }

            if (!Remove)
                PC->WorldInventory->Inventory.MarkItemDirty(PC->WorldInventory->Inventory.ReplicatedEntries[i]);
                
            break;
        }
    }

    if (Remove)
    {
        for (size_t i = 0; i < PC->WorldInventory->Inventory.ItemInstances.Num(); i++)
        {
            if (PC->WorldInventory->Inventory.ItemInstances[i]->GetItemDefinitionBP() == Def)
            {
                PC->WorldInventory->Inventory.ItemInstances.RemoveSingle(i);
                break;
            }
        }
    }

    PC->WorldInventory->bRequiresLocalUpdate = true;
    PC->WorldInventory->HandleInventoryLocalUpdate();
    if (Remove)
        PC->WorldInventory->Inventory.MarkArrayDirty();
}

void Abilities::InitAbilitiesForPlayer(SDK::AFortPlayerController* PC)
{
    auto PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;
    static auto AbilitySet = Utils::StaticLoadObject<UFortAbilitySet>("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_AthenaPlayer.GAS_AthenaPlayer");

    if (PlayerState && AbilitySet)
    {
        for (size_t i = 0; i < AbilitySet->GameplayAbilities.Num(); i++)
        {
            Utils::Log(AbilitySet->GameplayAbilities[i].Get()->GetName());
            FGameplayAbilitySpec wow{};
            AbilitySpecCtor(&wow, (UGameplayAbility*)AbilitySet->GameplayAbilities[i].Get()->DefaultObject, 1, -1, nullptr);
            GiveAbility(PlayerState->AbilitySystemComponent, &wow.Handle, wow);
        }
    }
}

FGameplayAbilitySpec* FindAbilitySpecFromHandle(UFortAbilitySystemComponentAthena* ASC, FGameplayAbilitySpecHandle& Handle)
{
    for (size_t i = 0; i < ASC->ActivatableAbilities.Items.Num(); i++)
    {
        if (ASC->ActivatableAbilities.Items[i].Handle.Handle == Handle.Handle)
            return &ASC->ActivatableAbilities.Items[i];
    }
    return nullptr;
}

void Accolades::GiveAccolade(AFortPlayerControllerAthena* PC, UFortAccoladeItemDefinition* Def)
{
    if (!Def)
        return;
    //for (size_t i = 0; i < PC->XPComponent->PlayerAccolades.Num(); i++)
    //{
    //    if (PC->XPComponent->PlayerAccolades[i].AccoladeDef == Def)
    //        return;
    //}
    FAthenaAccolades Accolade{};
    Accolade.AccoladeDef = Def;
    Accolade.Count = 1;
    string DefName = Def->GetName();
    Accolade.TemplateId = wstring(DefName.begin(), DefName.end()).c_str();

    auto ID = UKismetSystemLibrary::GetDefaultObj()->GetPrimaryAssetIdFromObject(Def);

    FXPEventInfo EventInfo{};
    EventInfo.Accolade = ID;
    EventInfo.EventName = Def->Name;
    EventInfo.EventXpValue = Def->GetAccoladeXpValue();
    EventInfo.Priority = Def->Priority;
    EventInfo.SimulatedText = Def->GetShortDescription();
    EventInfo.RestedValuePortion = EventInfo.EventXpValue;
    EventInfo.RestedXPRemaining = EventInfo.EventXpValue;
    EventInfo.SeasonBoostValuePortion = 20;
    EventInfo.TotalXpEarnedInMatch = EventInfo.EventXpValue + PC->XPComponent->TotalXpEarned;

    PC->XPComponent->MedalBonusXP += 1250;
    PC->XPComponent->MatchXp += EventInfo.EventXpValue;
    PC->XPComponent->TotalXpEarned += EventInfo.EventXpValue + 1250;

    PC->XPComponent->PlayerAccolades.Add(Accolade);
    PC->XPComponent->MedalsEarned.Add(Def);

    PC->XPComponent->ClientMedalsRecived(PC->XPComponent->PlayerAccolades);
    PC->XPComponent->OnXPEvent(EventInfo);
}

void InternalServerTryActivateAbilityHook(UFortAbilitySystemComponentAthena* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle, bool InputPressed, FPredictionKey& PredictionKey, FGameplayEventData* TriggerEventData) // Broo what is this thing
{
    FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(AbilitySystemComponent, Handle);
    if (!Spec)
        return AbilitySystemComponent->ClientActivateAbilityFailed(Handle, PredictionKey.Current);

    // Consume any pending target info, to clear out cancels from old executions
    //ConsumeAllReplicatedData(Handle, PredictionKey);

    UGameplayAbility* AbilityToActivate = Spec->Ability;

    UGameplayAbility* InstancedAbility = nullptr;
    Spec->InputPressed = true;

    // Attempt to activate the ability (server side) and tell the client if it succeeded or failed.
    if (!InternalTryActivateAbility(AbilitySystemComponent, Handle, PredictionKey, &InstancedAbility, nullptr, TriggerEventData))
    {
        AbilitySystemComponent->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
        Spec->InputPressed = false;
    }
    AbilitySystemComponent->ActivatableAbilities.MarkItemDirty(*Spec);
    //MarkAbilitySpecDirty(*Spec);
}

void Bots::SpawnBots()
{
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetDefaultObj()->GetAllActorsOfClass(UWorld::GetWorld(), AFortPlayerStartWarmup::StaticClass(), &PlayerStarts);

    for (size_t i = 0; i < 1; i++)
    {
        AActor* SpawnLocator = PlayerStarts[UKismetMathLibrary::GetDefaultObj()->RandomIntegerInRange(0, PlayerStarts.Num() - 1)];
        
        if (SpawnLocator)
        {
            Utils::Log("Spawning New Bot");
            SpawnedBots.push_back(new Bot(SpawnLocator));
        }
    }
    PlayerStarts.FreeArray();
}

uint8 Teams::PickTeam(SDK::AFortGameModeAthena* Gamemode, uint8 PreferredTeam, SDK::AFortPlayerControllerAthena* PC)
{
    uint8 Ret = NextIdx;
    CurrentPlayersOnTeam++;

    if (CurrentPlayersOnTeam == MaxPlayersOnTeam)
    {
        NextIdx++;
        CurrentPlayersOnTeam = 0;
    }
    return Ret;
}

void Abilities::InitHooks()
{
    Utils::Log("Initializing Abilities Hooks...");
    Utils::SwapVTable(UAbilitySystemComponent::GetDefaultObj(), 0xF7, InternalServerTryActivateAbilityHook);
    Utils::SwapVTable(UFortAbilitySystemComponent::GetDefaultObj(), 0xF7, InternalServerTryActivateAbilityHook);
    Utils::SwapVTable(UFortAbilitySystemComponentAthena::GetDefaultObj(), 0xF7, InternalServerTryActivateAbilityHook);

    static auto Base = __int64(GetModuleHandleW(0));

    MH_CreateHook((LPVOID)(Base + 0x153B210), Teams::PickTeam, nullptr);
    MH_EnableHook((LPVOID)(Base + 0x153B210));
    MH_CreateHook((LPVOID)(Base + 0x174AAA0), Utils::SpawnLoot, nullptr);
    MH_EnableHook((LPVOID)(Base + 0x174AAA0));
}