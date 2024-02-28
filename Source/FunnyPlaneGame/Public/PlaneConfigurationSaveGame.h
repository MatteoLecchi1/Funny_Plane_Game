// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlanePawn.h"
#include "PlaneConfigurationSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API UPlaneConfigurationSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString SaveSlotName;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
    FString SavedPlane;
    UPROPERTY(VisibleAnywhere, Category = Basic)
    TArray<FString> SavedHardpointWeapons = {"","","","","","","",""};

    UPlaneConfigurationSaveGame();
};
