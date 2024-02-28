// Fill out your copyright notice in the Description page of Project Settings.


#include "FunnyPlaneGameController.h"
#include "Kismet/GameplayStatics.h"
#include "PlaneConfigurationSaveGame.h"

void AFunnyPlaneGameController::BeginPlay()
{
	Super::BeginPlay();
	SaveInstance = Cast<UPlaneConfigurationSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveInstance->StaticClass()));
	SaveInstance = Cast<UPlaneConfigurationSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveInstance->SaveSlotName, SaveInstance->UserIndex));

	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, SaveInstance->SavedPlane);
	for (FString a :SaveInstance->SavedHardpointWeapons)
		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Orange, a);
}


void AFunnyPlaneGameController::SavePlaneByName(FString PlaneName)
{
	SaveInstance->SavedPlane = PlaneName;
	UGameplayStatics::SaveGameToSlot(SaveInstance, SaveInstance->SaveSlotName, SaveInstance->UserIndex);
}

void AFunnyPlaneGameController::SaveWeaponByNameAndHardpoint(FString WeaponName, int HardpointIndex)
{
	SaveInstance->SavedHardpointWeapons[HardpointIndex] = WeaponName;
	UGameplayStatics::SaveGameToSlot(SaveInstance, SaveInstance->SaveSlotName, SaveInstance->UserIndex);
}