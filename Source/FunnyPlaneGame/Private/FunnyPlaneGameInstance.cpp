// Fill out your copyright notice in the Description page of Project Settings.

#include "FunnyPlaneGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PlaneConfigurationSaveGame.h"
#include "PlaneDefinition.h"


UFunnyPlaneGameInstance* UFunnyPlaneGameInstance::GetGameInstance(const UObject* WorldContextObject)
{
	return Cast<UFunnyPlaneGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
}

void UFunnyPlaneGameInstance::Init()
{
	Super::Init();

	SaveInstance = Cast<UFunnyPlaneGameSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SlotName"), 0));

	// If savegame not present create an empty one
	if (SaveInstance == nullptr)
	{
		SaveInstance = Cast<UFunnyPlaneGameSaveGame>(UGameplayStatics::CreateSaveGameObject(UFunnyPlaneGameSaveGame::StaticClass()));
		PlanesDataTable->ForeachRow<FPlaneDefinition>("Plane", [&](const FName& Key, const FPlaneDefinition& PlaneDefinition) {
			auto& Plane = SaveInstance->PlaneConfig.SavedPlanes.FindOrAdd(Key);
			Plane.PlaneKey = Key; 
			UE_LOG(LogTemp, Warning, TEXT("%s"), * Key.ToString());
		}); 

		MissionsDataTable->ForeachRow<FMissionDefinition>("Mission", [&](const FName& Key, const FMissionDefinition& MissionDefinition) {
			auto& Mission = SaveInstance->UnlockedMissions.FindOrAdd(Key);
			Mission.MissionKey = Key;
			Mission.IsUnlocked = MissionDefinition.IsUnlocked;
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Key.ToString());
		});
	}
}


void UFunnyPlaneGameInstance::SavePlaneByName(FName PlaneKey)
{
	SaveInstance->PlaneConfig.CurrentPlaneKey = PlaneKey;
	UGameplayStatics::SaveGameToSlot(SaveInstance, TEXT("SlotName"), 0);
}

void UFunnyPlaneGameInstance::SaveWeaponByNameAndHardpoint(FName WeaponName, int HardpointIndex)
{
	if (auto CurrentPlane = SaveInstance->PlaneConfig.GetCurrentPlane())
	{
		// Resize SavedHardpointWeapons array if HardpointIndex is not present
		if (!CurrentPlane->SavedHardpointWeapons.IsValidIndex(HardpointIndex))
		{
			CurrentPlane->SavedHardpointWeapons.SetNum(HardpointIndex + 1);
		}
		CurrentPlane->SavedHardpointWeapons[HardpointIndex] = WeaponName;
		UGameplayStatics::SaveGameToSlot(SaveInstance, TEXT("SlotName"), 0);
	}
}

FSavedPlane& UFunnyPlaneGameInstance::GetCurrentPlane()
{
	return SaveInstance->PlaneConfig.SavedPlanes[SaveInstance->PlaneConfig.CurrentPlaneKey];
}
void UFunnyPlaneGameInstance::UnlockMission(FName MissionKey) 
{
	SaveInstance->UnlockedMissions[MissionKey].IsUnlocked = true;
	UGameplayStatics::SaveGameToSlot(SaveInstance, TEXT("SlotName"), 0);
}