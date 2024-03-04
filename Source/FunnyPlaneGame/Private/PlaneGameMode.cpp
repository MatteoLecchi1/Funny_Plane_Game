// Fill out your copyright notice in the Description page of Project Settings.
#include "PlaneGameMode.h"
#include "FunnyPlaneGameInstance.h"
#include "PlaneDefinition.h"

UClass* APlaneGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	auto GameInstance = UFunnyPlaneGameInstance::GetGameInstance(GetWorld());

	auto Row = GameInstance->PlanesDataTable->FindRow<FPlaneDefinition>(GameInstance->SaveInstance->CurrentPlaneKey, TEXT("Plane"));
	if (Row && Row->PlaneReferance)
	{
		return Row->PlaneReferance;
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}
