// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MissionSelectionListEntry.h"
#include "FunnyPlaneGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UMissionSelectionListEntry::OpenMission(FName LevelToOpen, FMissionDefinition MissionDefinition)
{
	auto GameInstance = UFunnyPlaneGameInstance::GetGameInstance(GetWorld());
	GameInstance->CurrentMission = MissionDefinition;

	UGameplayStatics::OpenLevel(GetWorld(), LevelToOpen);
}