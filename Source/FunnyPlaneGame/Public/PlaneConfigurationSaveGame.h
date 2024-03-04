// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlanePawn.h"
#include "PlaneConfigurationSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FUNNYPLANEGAME_API FSavedPlane
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = Plane)
	FName PlaneKey;

	UPROPERTY(BlueprintReadWrite, Category = Plane)
	TArray<FName> SavedHardpointWeapons;
};

UCLASS()
class FUNNYPLANEGAME_API UPlaneConfigurationSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FName CurrentPlaneKey;

    UPROPERTY(VisibleAnywhere, Category = Basic)
	TMap<FName,FSavedPlane> SavedPlanes;

};
