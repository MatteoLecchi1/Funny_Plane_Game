// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlaneDefinition.h"
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
};
