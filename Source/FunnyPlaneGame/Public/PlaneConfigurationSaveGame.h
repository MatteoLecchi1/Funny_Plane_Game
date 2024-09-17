// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
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

USTRUCT(BlueprintType)
struct FUNNYPLANEGAME_API FPlaneConfigurationSaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FName CurrentPlaneKey;

    UPROPERTY(VisibleAnywhere, Category = Basic)
	TMap<FName,FSavedPlane> SavedPlanes;

	FSavedPlane* GetCurrentPlane();
};
USTRUCT(BlueprintType)
struct FUNNYPLANEGAME_API FSavedMission
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = Plane)
	FName MissionKey;

	UPROPERTY(BlueprintReadWrite, Category = Plane)
	bool IsUnlocked;
};
UCLASS()
class FUNNYPLANEGAME_API UFunnyPlaneGameSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TMap<FName, FSavedMission> UnlockedMissions;

	UPROPERTY()
	FPlaneConfigurationSaveGame PlaneConfig;
};