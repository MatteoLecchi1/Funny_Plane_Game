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

	SaveInstance = Cast<UPlaneConfigurationSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SlotName"), 0));

	// If savegame not present create an empty one
	if (SaveInstance == nullptr)
	{
		SaveInstance = Cast<UPlaneConfigurationSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlaneConfigurationSaveGame::StaticClass()));
		PlanesDataTable->ForeachRow<FPlaneDefinition>("Plane", [&](const FName& Key, const FPlaneDefinition& PlaneDefinition) {
			auto& Plane = SaveInstance->SavedPlane.FindOrAdd(Key);
			Plane.PlaneKey = Key; 
			UE_LOG(LogTemp, Warning, TEXT("%s"), * Key.ToString());
		});
	}
}


void UFunnyPlaneGameInstance::SavePlaneByName(FString PlaneName)
{
	//SaveInstance->SavedPlane = PlaneName;
	UGameplayStatics::SaveGameToSlot(SaveInstance, TEXT("SlotName"), 0);
}

void UFunnyPlaneGameInstance::SaveWeaponByNameAndHardpoint(FString WeaponName, int HardpointIndex)
{
	//SaveInstance->SavedHardpointWeapons[HardpointIndex] = WeaponName;
	UGameplayStatics::SaveGameToSlot(SaveInstance, TEXT("SlotName"), 0);
}