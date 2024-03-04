// Fill out your copyright notice in the Description page of Project Settings.

#include "FunnyPlaneGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PlaneConfigurationSaveGame.h"

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
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, SaveInstance->SavedPlane.ToString());
	for (FString a : SaveInstance->SavedHardpointWeapons)
		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Orange, a);
}


void UFunnyPlaneGameInstance::SavePlaneByName(FName PlaneName)
{
	SaveInstance->SavedPlane = PlaneName;
	UGameplayStatics::SaveGameToSlot(SaveInstance, TEXT("SlotName"), 0);
}

void UFunnyPlaneGameInstance::SaveWeaponByNameAndHardpoint(FString WeaponName, int HardpointIndex)
{
	SaveInstance->SavedHardpointWeapons[HardpointIndex] = WeaponName;
	UGameplayStatics::SaveGameToSlot(SaveInstance, TEXT("SlotName"), SaveInstance->UserIndex);
}