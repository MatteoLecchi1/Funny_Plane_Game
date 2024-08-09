#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MissionDefinition.generated.h"

UENUM(BlueprintType)
enum class ObjectiveType : uint8
{
	DESTROYALLENEMIES = 0	UMETA(DisplayName = "DESTROY ALL ENEMIES"),
	DESTROYSPECIFICENEMIES = 1	UMETA(DisplayName = "DESTROY SPECIFIC ENEMIES"),
};

USTRUCT(BlueprintType)
struct FUNNYPLANEGAME_API FObjectiveDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective)
	ObjectiveType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective)
	FName ObjectiveTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective)
	FName SpawnerTags;
};

USTRUCT(BlueprintType)
struct FUNNYPLANEGAME_API FMissionDefinition : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mission)
	FString	Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mission)
	float IconNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mission)
	FString	Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mission)
	FString	LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mission)
	TArray<FObjectiveDefinition> Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mission)
	TArray<FName> MissionsToUnlock;
};



