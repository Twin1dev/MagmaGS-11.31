#pragma once
#include "Utils.h" // Include necessary header file

// Define a struct to hold consumable definitions and quantities
struct ConsumableInfo {
    UFortItemDefinition* Definition;
    int Quantity;
};

// Function to initialize consumable definitions
TArray<ConsumableInfo> InitializeConsumableDefinitions() {
    // Create an array to hold ConsumableInfo
    TArray<ConsumableInfo> ConsumableDefinitions;

    // Add consumables with their quantities
    ConsumableInfo SmallShieldInfo;
    SmallShieldInfo.Definition = Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall");
    SmallShieldInfo.Quantity = 3;
    ConsumableDefinitions.Add(SmallShieldInfo);

    ConsumableInfo MedkitInfo;
    MedkitInfo.Definition = Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Medkit/Athena_Medkit.Athena_Medkit");
    MedkitInfo.Quantity = 1;
    ConsumableDefinitions.Add(MedkitInfo);

    ConsumableInfo BandageInfo;
    BandageInfo.Definition = Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Bandage/Athena_Bandage.Athena_Bandage");
    BandageInfo.Quantity = 5;
    ConsumableDefinitions.Add(BandageInfo);

    ConsumableInfo ShieldsInfo;
    ShieldsInfo.Definition = Utils::StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields");
    ShieldsInfo.Quantity = 1;
    ConsumableDefinitions.Add(ShieldsInfo);

    return ConsumableDefinitions;
}
