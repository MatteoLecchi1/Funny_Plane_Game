// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlaneDefinition.h"
#include "MissionDefinition.h"
#include "UI/MissionSelectionUI.h"
#include "MissionSelectionListEntry.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API UMissionSelectionListEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = UI)
	FMissionDefinition Mission;

	UFUNCTION(BlueprintCallable)
	void OpenMission(FName LevelToOpen, FMissionDefinition MissionDefinition);
};
